#pragma once

#include "AbObject.h"

class SphereObject : public AbObject
{
public:
	SphereObject(QVector3D in_c, double in_r);
	~SphereObject();
	
	// intersection operators
	bool RayHitTest(Ray in_ray, QVector3D& out_point, QVector3D& out_normal);

private:
	// sphere parameters
	QVector3D		s_center_;
	double			s_radius_;
};

