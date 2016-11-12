#include "SphereObject.h"
#include "Ray.h"

SphereObject::SphereObject(QVector3D in_c, double in_r) :
s_center_(in_c),
s_radius_(in_r)
{
}


SphereObject::~SphereObject()
{
}


// intersection operators
bool SphereObject::RayHitTest(Ray in_ray, QVector3D& out_point, QVector3D& out_normal){
	// calculate the intersection with equations of ray & sphere
	QVector3D to_v = in_ray.OriginPoint() - s_center_;
	double to_b = QVector3D::dotProduct(in_ray.Direction(), to_v);
	double to_det = to_b * to_b - QVector3D::dotProduct(to_v, to_v) + s_radius_;
	bool ishit = false;

	if (to_det > 0)
	{

		to_det = sqrtf(to_det);
		double to_slu1 = -to_b - to_det;
		double to_slu2 = -to_b + to_det;
		if (to_slu1 >= 0)
		{
			out_point = in_ray.GetVertex(to_slu1);
			out_normal = (out_point - s_center_).normalized();
			ishit = true;
		}
		else if (to_slu2 >0)
		{
			out_point = in_ray.GetVertex(to_slu2);
			out_normal = (out_point - s_center_).normalized();
			ishit = true;
		}
	}

	return ishit;
}
