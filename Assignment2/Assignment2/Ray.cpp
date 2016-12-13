#pragma once
#include "Ray.h"

Ray::~Ray()
{
}

// set parameters
void Ray::SetOriginPoint(QVector3D in_o){
	r_origin_ = in_o;
}
void Ray::SetDirection(QVector3D in_d){
	r_direction_ = in_d.normalized();
}


QVector3D Ray::GetVertex(double in_t){
	QVector3D out_p = in_t * r_direction_ + r_origin_;
	return out_p;
}

double Ray::DistancetoPoint(const QVector3D& in_p){
	return in_p.distanceToLine(r_origin_, r_direction_);
}

