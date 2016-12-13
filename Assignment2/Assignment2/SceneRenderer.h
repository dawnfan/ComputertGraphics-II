#pragma once
#include <QVector>
#include "AbObject.h"


class Ray;
class Camera;
class AbObject;
class CubeLight;
class LightSource;

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	// obtain the things in my scene
	Camera* SceneCamera(){ return s_camera_; }
	QVector<LightSource*>* SceneLightList(){ return s_light_list_; }
	QVector<AbObject*>* SceneObjectList(){ return s_object_list_; }

	// rendering operations
	QVector3D GetColor(double in_x, double in_y);

private:
	// members in a scene
	Camera*					s_camera_;
	QVector<LightSource*>*	s_light_list_;
	QVector<AbObject*>*		s_object_list_;
	QVector3D				s_amibentcolor_;
	// environment lighting
	CubeLight*				s_cubelight_;
	
	QVector<QVector3D>		light_sample_cache;
	QVector<QVector3D>		light_sample_cache_color;

	void initScene();
	void createObjects();
	// intersections
	bool RayHitObject(Ray in_ray, IntersectPoint& out_point);
	bool isShadow();

	QVector3D getDiffuseColor(Ray* in_ray, IntersectionPoint hit_point);

	// check color range
	void validColor(QVector3D& in_color);
};

