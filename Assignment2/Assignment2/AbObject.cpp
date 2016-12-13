#include "AbObject.h"
#include "Ray.h"

void float_swap(float a, float b){float c=a;a=b;b=c;}

bool AABB::RayHitTest(Ray in_ray){
	QVector3D inv_dir = QVector3D(1.0f / in_ray.Direction().x(), 1.0f / in_ray.Direction().y(), 1.0f / in_ray.Direction().z());
	QVector3D tMin = (this->BB_Min - in_ray.OriginPoint()) * inv_dir;
	QVector3D tMax = (this->BB_Max - in_ray.OriginPoint()) * inv_dir;

	QVector3D t1 = QVector3D(std::min(tMin.x(), tMax.x()), std::min(tMin.y(), tMax.y()), std::min(tMin.z(), tMax.z()));
	QVector3D t2 = QVector3D(std::max(tMin.x(), tMax.x()), std::max(tMin.y(), tMax.y()), std::max(tMin.z(), tMax.z()));
	float tNear = std::max(std::max(t1.x(), t1.y()), t1.z());
	float tFar = std::min(std::min(t2.x(), t2.y()), t2.z());
	if (tNear > tFar)
		return false;
	else
		return true;
}

AbObject::AbObject()
{};
AbObject::~AbObject()
{};

// intersection operators
bool AbObject::RayHitTest(Ray, IntersectPoint&){
	return false;
};

QVector3D AbObject::SurfaceColor(){
	return o_surfacecolor_;
}