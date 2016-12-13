#include "SphereObject.h"
#include "Ray.h"

SphereObject::SphereObject(QVector3D in_c, double in_r, QVector3D in_color) :
s_center_(in_c),
s_radius_(in_r)
{
	o_surfacecolor_ = in_color;
}


SphereObject::~SphereObject()
{
}


// intersection operators
bool SphereObject::RayHitTest(Ray in_ray, IntersectPoint& out_hit){
	// calculate the intersection with equations of ray & sphere
	bool ishit = false;

	// quadratic equation
	QVector3D to_v = in_ray.OriginPoint() - s_center_;
	double to_a = QVector3D::dotProduct(in_ray.Direction(), in_ray.Direction());
	double to_b = 2 * QVector3D::dotProduct(in_ray.Direction(), to_v);
	double to_c = QVector3D::dotProduct(to_v, to_v) - s_radius_*s_radius_;
	double to_det = to_b * to_b - 4 * to_a*to_c;

	if (to_det >= 0)
	{
		to_det = sqrtf(to_det);
		double to_slu1 = (-to_b - to_det) / 2 /to_a;
		double to_slu2 = (-to_b + to_det) / 2 / to_a;
		if (to_slu1 > 1e-2)
		{
			out_hit.out_point = in_ray.GetVertex(to_slu1);
			out_hit.out_normal = (out_hit.out_point - s_center_).normalized();
			out_hit.out_color = o_surfacecolor_;
			ishit = true;
		}
		else if (to_slu2 > 1e-2)
		{
			out_hit.out_point = in_ray.GetVertex(to_slu2);
			out_hit.out_normal = (out_hit.out_point - s_center_).normalized();
			out_hit.out_color = o_surfacecolor_;
			ishit = true;
		}
	}
	if (ishit)
	{
		int x = 0;
	}
	return ishit;
}
