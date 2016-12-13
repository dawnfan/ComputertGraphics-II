#include <QVector>
#include "Ray.h"
#include "MeshObject.h"
#include "PlaneObject.h"
#include "ObjOps/objLoader.h"


MeshObject::MeshObject(QVector3D in_ct, QVector3D in_color):
m_center_(in_ct)
{
	o_surfacecolor_ = in_color;
	initObj();
}

MeshObject::~MeshObject()
{
}

void MeshObject::initObj(){
	m_objdata_ = new objLoader();
	int obj_error = m_objdata_->load("Resources/objs/bunny.obj");
	m_kdtree_ = new KDTree(m_center_ ,m_objdata_);
}

// intersection operators
bool IsInTriangle(const QVector3D A, const QVector3D B, const QVector3D C, const QVector3D D)
{
	// #1：重心法求交点是否在三角面片内部
	QVector3D v0 = C - A;
	QVector3D v1 = B - A;
	QVector3D v2 = D - A;

	float dot00 = QVector3D::dotProduct(v0, v0);
	float dot01 = QVector3D::dotProduct(v0, v1);
	float dot02 = QVector3D::dotProduct(v0, v2);
	float dot11 = QVector3D::dotProduct(v1, v1);
	float dot12 = QVector3D::dotProduct(v1, v2);

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly
	{
		return false;
	}

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly
	{
		return false;
	}

	return u + v <= 1;

///////////////////////// using angles////////////////////////////////////
//	QVector3D v1h = A - D; v1h.normalize();
//	QVector3D v2h = B - D; v2h.normalize();
//	QVector3D v3h = C - D; v3h.normalize();
//
//	//if point in triangle
//	double cos1 = QVector3D::dotProduct(v1h, v2h);
//	double cos2 = QVector3D::dotProduct(v2h, v3h);
//	double cos3 = QVector3D::dotProduct(v3h, v1h);
//
//	double theta1 = acos(cos1);
//	double theta2 = acos(cos2);
//	double theta3 = acos(cos3);
//#define M_PI 3.14159265358979323846
//	if (abs(2 * M_PI - (theta1 + theta2 + theta3)) < 1e-2)
//	{
//		return true;
//	}
///////////////////////////////////////////////////////////////////////

}

bool MeshObject::RayHitFace(obj_face* in_face, Ray in_ray, IntersectPoint& out_hit){
	bool ishit = false;
	// get the face vertexes
	QVector3D points_face[3];
	for (int i = 0; i < 3; i++)
	{
		obj_vector* vector_list = m_objdata_->vertexList[in_face->vertex_index[i]];
		points_face[i] = QVector3D(vector_list->e[0], vector_list->e[1], vector_list->e[2]);
	}
	// calculate the face normal
	QVector3D t1, t2, tN;
	t1 = points_face[1] - points_face[0];
	t2 = points_face[2] - points_face[0];
	//t1 = points_face[0] - points_face[1];
	//t2 = points_face[0] - points_face[2];
	tN = QVector3D::crossProduct(t1, t2);
	tN.normalize();
	// get the interaction point
	PlaneObject temp_face(points_face[0], tN);
	// 3：判断交点是否在三角面片内部
	if (temp_face.RayHitTest(in_ray, out_hit))
	{
		ishit = IsInTriangle(points_face[0], points_face[1], points_face[2], out_hit.out_point);
	}
	if (ishit)
	{
		hit_count++;
	}
	return ishit;
}

bool MeshObject::RayHitTest(Ray in_ray, IntersectPoint& out_hit){
	bool ishit = false;
	QVector3D ray_origin = in_ray.OriginPoint();
	double out_dist = MAX_FLOAT_VALUE;
	IntersectPoint temp_out;
	// test hit through KD-Tree
	//KDNode* hit_node = m_kdtree_->RayHitLeafNode(in_ray);
	m_kdtree_->RayHitLeafNode(in_ray);
	QVector<KDNode*> hit_list = m_kdtree_->GetHitNodes();
	for (QVector<KDNode*>::iterator hit_node = hit_list.begin(); hit_node != hit_list.end(); hit_node++)
	{
		QVector<int>* n_faces = (*hit_node)->GetFaceList();
		// test hit with faces in KD-node
		for (QVector<int>::iterator iface = n_faces->begin(); iface != n_faces->end(); iface++)
		{
			if (RayHitFace(m_objdata_->faceList[*iface], in_ray, temp_out))
			{
				ishit = true;
				double temp_dist = (temp_out.out_point - ray_origin).length();
				if (temp_dist < out_dist)
				{
					out_dist = temp_dist;
					out_hit = temp_out;
				}
			}
		}
	}
	out_hit.out_color = o_surfacecolor_;
	return ishit;
}

////////////////////////////// For KD-Tree //////////////////////////////////////
bool IsCollidAABB(AABB ab_1, AABB ab_2){
	bool iscoll = true;
	QVector3D coll_p1 = ab_2.BB_Max - ab_1.BB_Min;
	QVector3D coll_p2 = ab_2.BB_Min - ab_1.BB_Max;
	if (coll_p1.x() < 0 || coll_p1.y() < 0 || coll_p1.z() < 0)
	{
		iscoll = false;
	}
	else if (coll_p2.x() > 0 || coll_p2.y() > 0 || coll_p2.z() > 0)
	{
		iscoll = false;
	}
	//QVector3D rd1, rd2, pc1, pc2;
	//rd1 = (ab_1.BB_Max - ab_1.BB_Min) / 2;
	//rd2 = (ab_2.BB_Max - ab_2.BB_Min) / 2;
	//pc1 = (ab_1.BB_Max + ab_1.BB_Min) / 2;
	//pc2 = (ab_2.BB_Max + ab_2.BB_Min) / 2;
	//pc1 = pc2 - pc1;
	//rd1 = rd1 + rd2;
	//for (int i = 0; i < 3; i++)
	//{
	//	pc1[i] = pc1[i] < 0 ? -pc1[i] : pc1[i];
	//	if (pc1[i] > rd1[i])
	//	{
	//		return false;
	//	}
	//}
	return iscoll;
}

KDTree::KDTree(QVector3D in_ct,objLoader* in_obj) :
kd_center_(in_ct),
kd_objdata_(in_obj)
{
	kd_root_ = NULL;
	updateAABB();
	Unify(800);
	SetCenter(in_ct);
	BuildTree();
}
KDTree::~KDTree()
{
	// ToDo#1：追寻到叶子节点
	// ToDo#2：释放叶子节点的面片vector
}

AABB KDTree::updateAABB(){
	// record the AABB of root
	float xmax, xmin, ymax, ymin, zmax, zmin;
	xmax = ymax = zmax = MIN_FLOAT_VALUE;
	xmin = ymin = zmin = MAX_FLOAT_VALUE;
	// calculate the AABB
	obj_vector** vertex_list = kd_objdata_->vertexList;
	int vertex_count = kd_objdata_->vertexCount;
	for (int i = 0; i < vertex_count; i++)
	{
		// update AABB
		obj_vector* vertex_t = kd_objdata_->vertexList[i];
		xmax = max_ab(xmax, vertex_t->e[0]);
		ymax = max_ab(ymax, vertex_t->e[1]);
		zmax = max_ab(zmax, vertex_t->e[2]);
		xmin = min_ab(xmin, vertex_t->e[0]);
		ymin = min_ab(ymin, vertex_t->e[1]);
		zmin = min_ab(zmin, vertex_t->e[2]);
	}
	AABB root_ab;
	root_ab.BB_Max = QVector3D(xmax, ymax, zmax);
	root_ab.BB_Min = QVector3D(xmin, ymin, zmin);
	// update root if it is not empty
	if (kd_root_ != NULL)
	{
		kd_root_->SetAABB(root_ab);
	}
	kd_root_ab_ = root_ab;
	return root_ab;
}
void KDTree::initRoot(){
	QVector<int>* root_faceidx = new QVector<int>;
	// ToDo: store into root.
	//root_faceidx->push_back(i);
	int face_count = kd_objdata_->faceCount;
	for (int i = 0; i < face_count; i++)
	{
		root_faceidx->push_back(i);
	}
	//if (kd_root_ != NULL)
	//{
	//	delete kd_root_->GetFaceList();
	//}
	kd_root_ = new KDNode(kd_root_ab_, root_faceidx);
}

void KDTree::Unify(float size)
{	
	QVector3D scale_v = kd_root_ab_.BB_Max - kd_root_ab_.BB_Min;
	float scaleMax;

	if (scale_v.x() < scale_v.y())
	{
		scaleMax = scale_v.y();
	}
	else
	{
		scaleMax = scale_v.x();
	}
	if (scaleMax < scale_v.z())
	{
		scaleMax = scale_v.z();
	}
	float scaleV = size / scaleMax;
	QVector3D centerPos((kd_root_ab_.BB_Min.x() + kd_root_ab_.BB_Max.x()) / 2.f,
		(kd_root_ab_.BB_Min.y() + kd_root_ab_.BB_Max.y()) / 2.f,
		(kd_root_ab_.BB_Min.z()));
	for (size_t i = 0; i != kd_objdata_->vertexCount; i++)
	{
		obj_vector* cur_vertex = kd_objdata_->vertexList[i];
		for (int t = 0; t < 3; t++)
		{
			cur_vertex->e[t] -= centerPos[t];
			//cur_vertex->e[t] -= kd_center_[t];
			cur_vertex->e[t] *= scaleV;
		}
	}
	// update root AABB
	kd_root_ab_.BB_Max -= centerPos;
	kd_root_ab_.BB_Min -= centerPos;
	kd_root_ab_.BB_Max *= scaleV;
	kd_root_ab_.BB_Min *= scaleV;
}

void KDTree::BuildTree(){
	initRoot();
	SplitNode(kd_root_);
}

AABB KDTree::GetFaceAABB(obj_face* in_face){
	obj_vector** vertex_list = kd_objdata_->vertexList;
	// record the AABB of face
	float xmax, xmin, ymax, ymin, zmax, zmin;
	xmax = ymax = zmax = MIN_FLOAT_VALUE;
	xmin = ymin = zmin = MAX_FLOAT_VALUE;
	// calculate Bounding-box of the triangle
	for (int i = 0; i < 3; i++)
	{
		obj_vector* vertex_t = vertex_list[in_face->vertex_index[i]];
		xmax = max_ab(xmax, vertex_t->e[0]);
		ymax = max_ab(ymax, vertex_t->e[1]);
		zmax = max_ab(zmax, vertex_t->e[2]);
		xmin = min_ab(xmin, vertex_t->e[0]);
		ymin = min_ab(ymin, vertex_t->e[1]);
		zmin = min_ab(zmin, vertex_t->e[2]);
	}
	AABB out_ab;
	out_ab.BB_Max = QVector3D(xmax, ymax, zmax);
	out_ab.BB_Min = QVector3D(xmin, ymin, zmin);
	return out_ab;
}
void KDTree::SplitNode(KDNode* in_node){
	QVector<int>* n_faces = in_node->GetFaceList();
	if (n_faces == NULL)
	{
		in_node->IsLeaf(true);
		return;
	}
	if (n_faces->size() <= KDTREE_FACE_NUM_MIN)
	{
		in_node->IsLeaf(true);
		return;
	}
	AABB n_ab = in_node->GetAABB();
	int n_d = in_node->GetDegree();
	// splitting
	AABB child_ab[2];
	child_ab[0] = child_ab[1] = n_ab;
	QVector<int>* child_faceidx[2];
	child_faceidx[0] = new QVector<int>;
	child_faceidx[1] = new QVector<int>;
	// axis-aligned splitting X-Y-Z
	int split_axis = n_d % 3;
	// split the Bounding-Box
	double split_value = (n_ab.BB_Max[split_axis] + n_ab.BB_Min[split_axis]) / 2;
	child_ab[0].BB_Min[split_axis] = split_value;
	child_ab[1].BB_Max[split_axis] = split_value;
	// split the faces
	for (QVector<int>::iterator iface = n_faces->begin(); iface != n_faces->end(); iface++)
	{
		obj_face* cur_face = kd_objdata_->faceList[(*iface)];
		if (IsCollidAABB(child_ab[0], GetFaceAABB(cur_face)))
		{
			child_faceidx[0]->push_back(*iface);
		}
		if (IsCollidAABB(child_ab[1], GetFaceAABB(cur_face)))
		{
			child_faceidx[1]->push_back(*iface);
		}
	}

	//////////////////////////////Test for face Num///////////////////////////////////
	//int face_count = n_faces->size();
	//for (int i = face_count; i > -1; i--)
	//{
	//	obj_face* cur_face = kd_objdata_->faceList[n_faces->at(i)];
	//	if (IsCollidAABB(child_ab[0], GetFaceAABB(cur_face)))
	//	{
	//		child_faceidx[0]->push_back(n_faces->at(i));
	//	}
	//	if (IsCollidAABB(child_ab[1], GetFaceAABB(cur_face)))
	//	{
	//		child_faceidx[1]->push_back(n_faces->at(i));
	//	}
	//	n_faces->pop_back();
	//}
	/////////////////////////////////////////////////////////////////////////////////

	// restore into children nodes
	in_node->SetChild(new KDNode(child_ab[0], child_faceidx[0], n_d + 1), 0);
	in_node->SetChild(new KDNode(child_ab[1], child_faceidx[1], n_d + 1), 1);
	// delete father's faces
	n_faces->clear();
	//delete n_faces;
	//n_faces = NULL;
	// recursion to children
	SplitNode(in_node->GetChild(0));
	SplitNode(in_node->GetChild(1));
	return;
}

void KDTree::SetCenter(QVector3D in_ct){
	kd_center_ = in_ct;
	QVector3D centerPos((kd_root_ab_.BB_Min.x() + kd_root_ab_.BB_Max.x()) / 2.f,
		(kd_root_ab_.BB_Min.y() + kd_root_ab_.BB_Max.y()) / 2.f,
		(kd_root_ab_.BB_Min.z()));
	QVector3D trans_pos = kd_center_ - centerPos;
	for (size_t i = 0; i != kd_objdata_->vertexCount; i++)
	{
		obj_vector* cur_vertex = kd_objdata_->vertexList[i];
		for (int t = 0; t < 3; t++)
		{
			cur_vertex->e[t] += trans_pos[t];
		}
	}
	// update AABB
	kd_root_ab_.BB_Max += trans_pos;
	kd_root_ab_.BB_Min += trans_pos;
}

void KDTree::RayHitLeafNode(Ray in_ray){
	if (kd_hitlist_.size() != 0)
	{
		kd_hitlist_.clear();
	}
	hitNode(in_ray, kd_root_);
}

void KDTree::hitNode(Ray in_ray, KDNode* in_node){
	//KDNode* hit_node = kd_root_;
	//while (!hit_node->IsLeaf())
	//{
	//	if (hit_node->GetChild(0)->GetAABB().RayHitTest(in_ray))
	//	{
	//		hit_node = hit_node->GetChild(0);
	//	}
	//	else
	//	{
	//		hit_node = hit_node->GetChild(1);
	//	}
	//}
	if (in_node->GetAABB().RayHitTest(in_ray))
	{
		if (in_node->IsLeaf())
		{
			kd_hitlist_.push_back(in_node);
			return;
		}
		else
		{
			hitNode(in_ray, in_node->GetChild(0));
			hitNode(in_ray, in_node->GetChild(1));
		}
	}
}