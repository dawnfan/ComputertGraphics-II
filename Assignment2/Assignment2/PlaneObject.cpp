#include "PlaneObject.h"
#include "Ray.h"

PlaneObject::PlaneObject(QVector3D in_p, QVector3D in_n):
p_point_(in_p),
p_normal_(in_n.normalized())
{
}


PlaneObject::~PlaneObject()
{
}

bool PlaneObject::RayHitTest(Ray in_ray, QVector3D& out_point, QVector3D& out_normal){
	// calculate the intersection with equations of plane & ray
	double to_parall = QVector3D::dotProduct(p_normal_, in_ray.Direction());
	bool ishit = false;
	
	if (to_parall != 0)
	{
		double to_d = QVector3D::dotProduct(p_normal_, p_point_ - in_ray.OriginPoint());
		to_d = to_d / to_parall;
		if (to_d > 0)
		{
			out_point = in_ray.GetVertex(to_d);
			out_normal = p_normal_;
			ishit = true;
		}
	}

	return ishit;
}
