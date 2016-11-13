#include "LightSource.h"
#include "Ray.h"

LightSource::LightSource(QVector3D in_ct, QVector3D in_cl):
l_center_(in_ct),
l_color_(in_cl)
{
}


LightSource::~LightSource()
{
}

// set lighting source parameters
void LightSource::SetCenter(QVector3D in_c){
	l_center_ = in_c;
}
void LightSource::SetColor(QVector3D in_c){
	l_color_ = in_c;
}

bool LightSource::RayHitTest(Ray in_ray){
	bool ishit = false;
	QVector3D to_v = l_center_ - in_ray.OriginPoint();
	to_v.normalize();
	QVector3D to_n = in_ray.Direction();
	to_v -= to_n;
	if (to_v.length() < 1e-2)
	{
		ishit = true;
	}
	return ishit;
}