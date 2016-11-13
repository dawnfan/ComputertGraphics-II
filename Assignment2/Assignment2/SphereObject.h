#pragma once

#include "AbObject.h"

class SphereObject : public AbObject
{
public:
	SphereObject(QVector3D in_c = QVector3D(0, 0, 400), double in_r = 400.0, QVector3D in_color = QVector3D(200, 200, 140));
	~SphereObject();
	
	// intersection operators
	bool RayHitTest(Ray in_ray, IntersectPoint& out_hit);

private:
	// sphere parameters
	QVector3D		s_center_;
	double			s_radius_;
};

