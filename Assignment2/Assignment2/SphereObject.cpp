#include "SphereObject.h"
#include "Ray.h"

SphereObject::SphereObject(QVector3D in_c, double in_r):
s_center_(in_c),
s_radius_(in_r)
{
}


SphereObject::~SphereObject()
{
}


// intersection operators
bool SphereObject::RayHitTest(Ray in_ray){
	// distance between the ray & its center
	double dis = in_ray.DistancetoPoint(s_center_);
	// is there a intersection
	if (dis > s_radius_)
	{
		return false;
	}
	return true;
}
