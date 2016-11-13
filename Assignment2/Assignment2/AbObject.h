#pragma once

#include <QVector3D>

class Ray;

typedef struct BoundingBox
{
	QVector3D		BB_Max;
	QVector3D		BB_Min;
} AABB;

typedef struct IntersectionPoint
{
	QVector3D out_point;
	QVector3D out_normal;
	QVector3D out_color;
	bool isLight = false;
}IntersectPoint;

class AbObject
{
public:
	AbObject();
	virtual ~AbObject();

	// intersection operators
	virtual bool RayHitTest(Ray, IntersectPoint&);
	QVector3D SurfaceColor();
	QVector3D		o_surfacecolor_;
};

