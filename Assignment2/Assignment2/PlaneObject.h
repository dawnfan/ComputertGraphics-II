#pragma once

#include "AbObject.h"

class PlaneObject : public AbObject
{
public:
	PlaneObject(QVector3D in_p = QVector3D(0, 1200, 100), QVector3D in_n = QVector3D(0, -1, 0), QVector3D in_c = QVector3D(250, 250, 250));
	~PlaneObject();
	// intersection operators
	bool RayHitTest(Ray in_ray, IntersectPoint& out_hit);

private:
	// sphere parameters
	QVector3D		p_point_;
	QVector3D		p_normal_;
};

