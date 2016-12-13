#pragma once
#include <QImage>

#include "Ray.h"
#include "CubeLight.h"
#include "PlaneObject.h"


CubeLight::CubeLight(int in_r) :
c_radius_(in_r)
{
	c_center_ = QVector3D(0, 0, 0);
	updatePlanes();
	initLightMap();
	// sampleing
	for (int i = 0; i < 6; i++)
	{
		c_sample_[i] = new QuadTree(c_colormap_ + i);
	}
}


CubeLight::~CubeLight()
{
}

void CubeLight::initLightMap(){
	lw::Image cubemap;
	cubemap.load("Resources/cubemaps/stpeters_cross.hdr");
	int it_width = cubemap.width() / 3;
	c_scalar_ = c_radius_ / it_width;

	// initialize the map size
	for (int i = 0; i < 6; i++)
	{
		c_colormap_[i].resize(it_width);
		//for (int j = 0; j < it_width; j++)
		//{
		//	(*(c_colormap_+i))[j].resize(it_width);
		//}
	}
	// set colors for every plane
	for (int w_step = 0; w_step < it_width; w_step++)
	{
		for (int h_step = 0; h_step < it_width; h_step++)
		{
			// top: x,z
			c_colormap_[TOP][w_step].push_back(cubemap.pixel(it_width + w_step, h_step));
			// left: y,z
			c_colormap_[LEFT][w_step].push_back(cubemap.pixel(w_step, it_width * 2 - h_step));
			// back: x,y
			c_colormap_[BACK][w_step].push_back(cubemap.pixel(it_width + w_step, it_width * 2 - h_step));
			// right: y,z
			c_colormap_[RIGHT][w_step].push_back(cubemap.pixel(it_width * 3 - 1 - w_step, it_width * 2 - h_step));
			// down: x,z
			c_colormap_[DOWN][w_step].push_back(cubemap.pixel(it_width + w_step, it_width * 3 - h_step));
			// front: x,y
			c_colormap_[FRONT][w_step].push_back(cubemap.pixel(it_width + w_step, it_width * 3 + h_step));
		}
	}
}

void CubeLight::updatePlanes(){
	// top
	c_planes_[TOP] = new PlaneObject(QVector3D(0, CENTER_L, 0), QVector3D(0, -1, 0));
	// left
	c_planes_[LEFT] = new PlaneObject(QVector3D(-CENTER_L, 0, 0), QVector3D(1, 0, 0));
	// back
	c_planes_[BACK] = new PlaneObject(QVector3D(0, 0, CENTER_L), QVector3D(0, 0, -1));
	// right
	c_planes_[RIGHT] = new PlaneObject(QVector3D(CENTER_L, 0, 0), QVector3D(-1, 0, 0));
	// down
	c_planes_[DOWN] = new PlaneObject(QVector3D(0, -CENTER_L, 0), QVector3D(0, 1, 0));
	// front
	c_planes_[FRONT] = new PlaneObject(QVector3D(0, 0, -CENTER_L), QVector3D(0, 0, 1));
}

QVector3D CubeLight::getColorFromMap(QVector3D in_p, FACE_DIR in_face){
	QVector3D out_color;
	// get the 2d position on the face
	int map_pos[2];
	switch (in_face)
	{
	case TOP:
	case DOWN:
		map_pos[0] = in_p.x();
		map_pos[1] = in_p.z();
		break;
	case LEFT:
	case RIGHT:
		map_pos[1] = in_p.y();
		map_pos[0] = in_p.z();
		break;
	case BACK:
	case FRONT:
		map_pos[0] = in_p.x();
		map_pos[1] = in_p.y();
		break;
	default:
		break;
	}
	map_pos[0] += CENTER_L - 1;
	map_pos[1] += CENTER_L - 1;
	// bilinear interpolation
	out_color = getColorFromMap(map_pos[0], map_pos[1], in_face);

	return out_color;
}

QVector3D CubeLight::getColorFromMap(int i, int j, FACE_DIR in_face){
	int map_pos[2] = { i, j };
	QVector3D out_color(0, 0, 0);
	int it_width = c_colormap_[0].size();
	// bilinear interpolation
	double inter_ratio[2];
	inter_ratio[0] = (double)(map_pos[0] % c_scalar_) / c_scalar_;
	inter_ratio[1] = (double)(map_pos[0] % c_scalar_) / c_scalar_;
	int light_pos[2] = { map_pos[0] / c_scalar_, map_pos[1] / c_scalar_ };
	lw::Color out_temp[2];
	if (light_pos[0] < c_colormap_[0].size() - 1 && light_pos[1] < c_colormap_[0].size() - 1)
	{
		out_temp[0] = c_colormap_[in_face][light_pos[0]][light_pos[1]] * inter_ratio[0] +
			c_colormap_[in_face][light_pos[0] + 1][light_pos[1]] * (1 - inter_ratio[0]);
		out_temp[1] = c_colormap_[in_face][light_pos[0]][light_pos[1] + 1] * inter_ratio[0] +
			c_colormap_[in_face][light_pos[0] + 1][light_pos[1] + 1] * (1 - inter_ratio[0]);
	}
	else
		out_temp[0] = out_temp[1] = c_colormap_[in_face][light_pos[0]][light_pos[1]];
	out_temp[0] = out_temp[0] * inter_ratio[0] + out_temp[1] * (1 - inter_ratio[0]);
	out_color = QVector3D(out_temp[0].red(), out_temp[0].green(), out_temp[0].blue());

	return out_color;
}

QVector3D CubeLight::getWorldPos(int i, int j, FACE_DIR in_face){
	QVector3D out_pos(CENTER_L, CENTER_L, CENTER_L);
	switch (in_face)
	{
	case DOWN:
		out_pos.setY(-CENTER_L);
	case TOP:
		out_pos.setX(i*c_scalar_ - CENTER_L);
		out_pos.setZ(j*c_scalar_ - CENTER_L);
		break;
	case LEFT:
		out_pos.setX(-CENTER_L);
	case RIGHT:
		out_pos.setY(j*c_scalar_ - CENTER_L);
		out_pos.setZ(i*c_scalar_ - CENTER_L);
		break;
	case FRONT:
		out_pos.setZ(-CENTER_L);
	case BACK:
		out_pos.setX(i*c_scalar_ - CENTER_L);
		out_pos.setY(j*c_scalar_ - CENTER_L);
		break;
	default:
		break;
	}
	return out_pos;
}
// set lighting parameters
void CubeLight::SetRadius(int in_r){
	c_radius_ = in_r;
	updatePlanes();
}

bool CubeLight::RayHitTest(Ray in_ray, IntersectionPoint& out_point){
	bool ishit = false;
	QVector3D ray_origin = in_ray.OriginPoint();
	double out_dist = MAX_FLOAT_VALUE;
	IntersectPoint temp_out;
	FACE_DIR faceid;
	for (int i = 0; i < 6; i++)
	{
		if (c_planes_[i]->RayHitTest(in_ray, temp_out))
		{
			ishit = true;
			double temp_dist = (temp_out.out_point - ray_origin).length();
			if (temp_dist < out_dist)
			{
				out_dist = temp_dist;
				out_point = temp_out;
				faceid = FACE_DIR(i);
			}
		}
	}

	out_point.out_color = getColorFromMap(out_point.out_point, faceid);
	return ishit;
}

QVector<QVector3D> CubeLight::GetSampleList(QVector<QVector3D>* out_color){
	QVector<QVector3D> out_list;
	if (!out_color->isEmpty())
	{
		out_color->clear();
	}
	for (int i = 0; i < 6; i++)
	{
		QVector<int> cur_list = c_sample_[i]->sample_list_;
		int sample_size = cur_list.size() / 2;
		for (int s_i = 0; s_i < sample_size; s_i++)
		{
			QVector3D light_pos = getWorldPos(cur_list[s_i], cur_list[s_i + 1], FACE_DIR(i));
			out_list.push_back(light_pos);
			out_color->push_back(getColorFromMap(cur_list[s_i], cur_list[s_i + 1], FACE_DIR(i)));
		}
	}
	return out_list;
}


//////////////////////// QuadTree for Sampling lights /////////////////////////////

//////////////////////////////////Node////////////////////////////////////////////
QuadNode::QuadNode(int in_region[2][2], int in_degree) :
qtn_degree_(in_degree)
{
	qtn_region_[0][0] = in_region[0][0];
	qtn_region_[0][1] = in_region[0][1];
	qtn_region_[1][0] = in_region[1][0];
	qtn_region_[1][1] = in_region[1][1];
	// initialize
	qtn_isleaf_ = false;
	for (int i = 0; i < 4; i++)
	{
		qtn_child_[i] = NULL;
	}
}

QuadNode::~QuadNode()
{
}
void QuadNode::GrowChidren(){
	int region_temp[2][2];
	int split_value[2] = { (qtn_region_[0][0] + qtn_region_[0][1]) / 2,
		(qtn_region_[1][0] + qtn_region_[1][1]) / 2 };
	region_temp[0][0] = qtn_region_[0][0];
	region_temp[0][1] = split_value[0];
	region_temp[1][0] = qtn_region_[1][0];
	region_temp[1][1] = split_value[1];
	qtn_child_[0] = new QuadNode(region_temp, qtn_degree_ + 1);
	region_temp[0][0] = split_value[0];
	region_temp[0][1] = qtn_region_[0][1];
	qtn_child_[1] = new QuadNode(region_temp, qtn_degree_ + 1);
	region_temp[0][0] = qtn_region_[0][0];
	region_temp[0][1] = split_value[0];
	region_temp[1][0] = split_value[1];
	region_temp[1][1] = qtn_region_[1][1];
	qtn_child_[2] = new QuadNode(region_temp, qtn_degree_ + 1);
	region_temp[0][0] = split_value[0];
	region_temp[0][1] = qtn_region_[0][1];
	qtn_child_[3] = new QuadNode(region_temp, qtn_degree_ + 1);
}
//////////////////////////////////Tree////////////////////////////////////////////
QuadTree::QuadTree(QVector<QVector<lw::Color> >* in_map) :
qt_map_(in_map)
{
	qt_root_ = NULL;
	updateRadianceMap();
	buildTree();
}

QuadTree::~QuadTree()
{
}

void QuadTree::buildTree(){
	// initialize root node
	int it_width = qt_map_->size();
	int it_height = qt_map_->at(0).size();
	int root_region[2][2];
	root_region[0][0] = 0;
	root_region[0][1] = it_width;
	root_region[1][0] = 0;
	root_region[1][1] = it_height;
	qt_root_ = new QuadNode(root_region, 0);
	// build it up with splitting
	splitNode(qt_root_);
}

void QuadTree::splitNode(QuadNode* in_node){
	double max_rad;
	double it_var = getVariance(qt_root_, max_rad);
	bool is_limit = (in_node->qtn_region_[0][1] - in_node->qtn_region_[0][1]) / 2 < QUAD_REGION_LIMIT;
	is_limit = is_limit || (in_node->qtn_region_[0][1] - in_node->qtn_region_[0][1]) / 2 < QUAD_REGION_LIMIT;
	if (it_var < QUAD_VARIANCE_THRESHOLD || is_limit)
	{
		in_node->IsLeaf(true);
		// do sampling
		doSampling(in_node, max_rad);
		return;
	}
	in_node->GrowChidren();
	for (int i = 0; i < 4; i++)
	{
		splitNode(in_node->GetChildren(i));
	}
}

void QuadTree::doSampling(QuadNode* in_node, double in_max){
	int w_step = in_node->qtn_region_[0][1] - in_node->qtn_region_[0][0],
		h_step = in_node->qtn_region_[1][1] - in_node->qtn_region_[1][0];
	int scalar_step = in_max / QUAD_SAMPLE_SEED;
	scalar_step = max_ab(scalar_step, 1);
	w_step /= scalar_step;
	h_step /= scalar_step;
	for (int i = in_node->qtn_region_[0][0]; i < in_node->qtn_region_[0][1]; i += w_step)
	{
		for (int j = in_node->qtn_region_[1][0]; j < in_node->qtn_region_[1][1]; j += h_step)
		{
			sample_list_.push_back(i);
			sample_list_.push_back(j);
		}
	}
}

void QuadTree::updateRadianceMap(){
	int it_width = qt_map_->size();
	int it_height = qt_map_->at(0).size();
	if (!radiance_map_.isEmpty())
	{
		radiance_map_.clear();
	}
	radiance_map_.resize(it_width);
	for (int w_step = 0; w_step < it_width; w_step++)
	{
		for (int h_step = 0; h_step < it_height; h_step++)
		{
			lw::Color temp_col = qt_map_->at(w_step)[h_step];
			radiance_map_[w_step].push_back(temp_col.red() + temp_col.green() + temp_col.blue());
		}
	}
}

double QuadTree::getVariance(QuadNode* in_node, double & out_max){
	double out_var = 0.0;
	double rad_max = MIN_FLOAT_VALUE;
	double rad_min = MAX_FLOAT_VALUE;
	for (int i = in_node->qtn_region_[0][0]; i < in_node->qtn_region_[0][1]; i++)
	{
		for (int j = in_node->qtn_region_[1][0]; j < in_node->qtn_region_[1][1]; j++)
		{
			rad_min = min_ab(rad_min, radiance_map_[i][j]);
			rad_max = max_ab(rad_max, radiance_map_[i][j]);
		}
	}

	out_var = rad_max - rad_min;
	out_max = rad_max;
	return out_var;
}
//////////////////////////////////////////////////////////////////////////////////