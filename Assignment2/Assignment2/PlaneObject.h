#pragma once

#include "AbObject.h"

class PlaneObject
{
public:
	PlaneObject(QVector3D in_p, QVector3D in_n);
	~PlaneObject();
	// intersection operators
	bool RayHitTest(Ray in_ray, QVector3D& out_point, QVector3D& out_normal);

private:
	// sphere parameters
	QVector3D		p_point_;
	QVector3D		p_normal_;
};

