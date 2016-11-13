#include "AbObject.h"
#include "Ray.h"

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