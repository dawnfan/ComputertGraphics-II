#include "PlaneObject.h"
#include "Ray.h"

PlaneObject::PlaneObject(QVector3D in_p, QVector3D in_n, QVector3D in_c):
p_point_(in_p),
p_normal_(in_n.normalized())
{
	//o_surfacecolor_ = QVector3D(140, 170, 200);
	o_surfacecolor_ = in_c;
}


PlaneObject::~PlaneObject()
{
}

bool PlaneObject::RayHitTest(Ray in_ray, IntersectPoint& out_hit){
	// calculate the intersection with equations of plane & ray
	double to_parall = QVector3D::dotProduct(p_normal_, in_ray.Direction());
	bool ishit = false;
	
	if (to_parall != 0)
	{
		double to_d = QVector3D::dotProduct(p_normal_, p_point_ - in_ray.OriginPoint());
		to_d = to_d / to_parall;
		if (to_d > 1e-2)
		{
			out_hit.out_point = in_ray.GetVertex(to_d);
			out_hit.out_normal = p_normal_;
			out_hit.out_color = o_surfacecolor_;
			ishit = true;
		}
	}

	return ishit;
}
