#pragma once
#include <QVector>

#include "PlaneObject.h"
#include "ImageOps/image.h"

#define CENTER_L c_radius_ / 2

enum FACE_DIR
{
	TOP,
	LEFT,
	BACK,
	RIGHT,
	DOWN,
	FRONT
};

class Ray;


//////////////////////// QuadTree for Sampling lights /////////////////////////////
#define QUAD_VARIANCE_THRESHOLD 1000
#define QUAD_SAMPLE_SEED 200
#define QUAD_REGION_LIMIT 50
class QuadNode
{
public:
	QuadNode(int in_region[2][2], int in_degree);
	~QuadNode();
	void IsLeaf(bool in_set){ qtn_isleaf_ = in_set; }
	bool IsLeaf(){ return qtn_isleaf_; }
	void GrowChidren();
	QuadNode* GetChildren(int i){ return qtn_child_[i]; }
	int			qtn_region_[2][2];
private:
	int			qtn_degree_;
	bool		qtn_isleaf_;
	QuadNode*	qtn_child_[4];
	// variance parameters //
	//double		qtn_variance_;
	//double		qtn_max_;
};
class QuadTree
{
public:
	QuadTree(QVector<QVector<lw::Color> >* in_map);
	~QuadTree();
	QVector<int>					sample_list_;

private:
	QVector<QVector<lw::Color> >*	qt_map_;
	QuadNode*						qt_root_;
	QVector<QVector<double> >		radiance_map_;
	void buildTree();
	void splitNode(QuadNode* in_node);
	void doSampling(QuadNode* in_node, double in_max);
	void updateRadianceMap();
	double getVariance(QuadNode* in_node, double & out_max);
};
//////////////////////////////////////////////////////////////////////////////////


class CubeLight
{
public:
	CubeLight(int in_r = 2560);
	~CubeLight();

	// lighting source parameters
	void SetRadius(int in_r);
	int GetRadius(){ return c_radius_; }

	// obtain lighting source parameters
	QVector3D LightCenter(){ return c_center_; };

	// intersection
	bool RayHitTest(Ray in_ray, IntersectionPoint& out_point);
	// diffuse lighting
	QVector<QVector3D> GetSampleList(QVector<QVector3D>* out_color);
private:
	int								c_radius_;
	//lw::Image*						c_cubemap_;
	QVector<QVector<lw::Color> >	c_colormap_[6];
	int								c_scalar_;
	QVector3D						c_center_;
	PlaneObject*					c_planes_[6];
	QuadTree*						c_sample_[6];

	void initLightMap();
	void updatePlanes();
	QVector3D getWorldPos(int i, int j, FACE_DIR in_face);
	QVector3D getColorFromMap(int i, int j, FACE_DIR in_face);
	QVector3D getColorFromMap(QVector3D in_p, FACE_DIR in_face);
};

