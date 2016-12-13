#pragma once

#include <QVector3D>


#define MAX_FLOAT_VALUE (static_cast<float>(10e10))
#define MIN_FLOAT_VALUE	(static_cast<float>(-10e10))
#define max_ab(a,b) a < b ? b : a
#define min_ab(a,b) a > b ? b : a

class Ray;

typedef struct BoundingBox
{
	QVector3D		BB_Max;
	QVector3D		BB_Min;
	bool RayHitTest(Ray in_ray);
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

