#pragma once

#include <QVector3D>

class Ray;

typedef struct BoundingBox
{
	QVector3D		BB_Max;
	QVector3D		BB_Min;
} AABB;

class AbObject
{
public:
	AbObject();
	virtual ~AbObject();

	// intersection operators
	virtual bool RayHitTest(Ray, QVector3D&, QVector3D&);

};

