#pragma once
#include <QVector3D>


class Ray
{
public:
	Ray(QVector3D o = QVector3D(0, 0, 0), QVector3D dir = QVector3D(0, 0, 1)) :r_origin_(o), r_direction_(dir.normalized()){};
	~Ray();

	// set parameters
	void SetOriginPoint(QVector3D in_o);
	void SetDirection(QVector3D in_d);
	
	// intersection operators
	QVector3D GetVertex(double in_t);
	double DistancetoPoint(const QVector3D& in_p);

private:
	// basic parameters
	QVector3D		r_origin_;
	QVector3D		r_direction_;
};

