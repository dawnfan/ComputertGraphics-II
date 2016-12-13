#pragma once
#include "AbObject.h"

class objLoader;
class obj_face;
template class QVector<int>;
class Ray;

#define KDTREE_FACE_NUM_MIN 20
class KDNode
{
public:
	KDNode(AABB in_ab, QVector<int>* in_faceidx, int in_d = 0) :
		kdn_ab_(in_ab),
		kdn_faceidx_(in_faceidx),
		kdn_degree_(in_d)
	{
		kdn_child_[0] = kdn_child_[1] = NULL;
		kdn_isleaf_ = false;
	};
	~KDNode(){};

	QVector<int>* GetFaceList(){ return kdn_faceidx_; }
	void SetAABB(AABB in_ab){ kdn_ab_ = in_ab; }
	AABB GetAABB(){ return kdn_ab_; }
	int GetDegree(){ return kdn_degree_; }
	bool IsLeaf(){ return kdn_isleaf_; }
	bool IsLeaf(bool to_set ){
		kdn_isleaf_ = to_set;
		return kdn_isleaf_;
	}
	KDNode*	GetChild(int i){ return kdn_child_[i]; }
	void SetChild(KDNode* in_node, int i){ kdn_child_[i] = in_node; }
private:
	int				kdn_degree_;
	AABB			kdn_ab_;
	KDNode*			kdn_child_[2];
	bool			kdn_isleaf_;
	QVector<int>*	kdn_faceidx_;
};
class KDTree
{
public:
	KDTree(QVector3D in_ct, objLoader* in_obj);
	~KDTree();

	void SetCenter(QVector3D in_ct);
	void BuildTree();
	void RayHitLeafNode(Ray in_ray);
	QVector<KDNode*> GetHitNodes(){ return kd_hitlist_; }
private:
	QVector3D			kd_center_;
	AABB				kd_root_ab_;
	objLoader*			kd_objdata_;
	KDNode*				kd_root_;
	QVector<KDNode*>	kd_hitlist_;
	//QVector<QVector3D>*	kd_faces_;

	AABB updateAABB();
	void initRoot();
	void SplitNode(KDNode* in_node);
	AABB GetFaceAABB(obj_face* in_face);
	void hitNode(Ray in_ray, KDNode* in_node);
	void Unify(float size);
};



class MeshObject :
	public AbObject
{
public:
	MeshObject(QVector3D in_ct = QVector3D(0,0,50), QVector3D in_color = QVector3D(200, 100, 100));
	~MeshObject();
	// intersection operators
	void SetPosition(QVector3D in_ct){ m_center_ = in_ct; m_kdtree_->SetCenter(in_ct); };
	bool RayHitTest(Ray in_ray, IntersectPoint& out_hit);
	int hit_count=0;
private:
	objLoader*			m_objdata_;
	KDTree*				m_kdtree_;
	QVector3D			m_center_;
	bool RayHitFace(obj_face* in_face, Ray in_ray, IntersectPoint& out_hit);
	void initObj();
};

