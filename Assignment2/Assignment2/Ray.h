#pragma once
#include <QVector3D>


class Ray
{
public:
	Ray(QVector3D& o, QVector3D& dir) :r_origin_(o), r_direction_(dir){};
	~Ray();

private:
	// basic parameters
	QVector3D		r_origin_;
	QVector3D		r_direction_;
};

