#include "AbObject.h"
#include "Ray.h"

AbObject::AbObject()
{};
AbObject::~AbObject()
{};

// intersection operators
bool AbObject::RayHitTest(Ray, QVector3D&, QVector3D&){
	return false;
};