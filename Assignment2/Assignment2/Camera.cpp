#include "Camera.h"
#include "Ray.h"

Camera::Camera(QSize in_cl, QSize in_ps, QVector3D in_ct, QVector3D in_vd, double in_fl, QVector3D in_up) :
c_clip_(in_cl),
c_pixelsize_(in_ps),
c_center_(in_ct),
c_viewdir_(in_vd.normalized()),
c_focallength_(in_fl),
c_up_(in_up.normalized())
{}

Camera::~Camera()
{
}

// set parameters
void Camera::SetClip(QSize in_c){
	c_clip_ = in_c;
}
void Camera::SetView(QVector3D in_v){
	c_viewdir_ = in_v.normalized();
}
void Camera::SetPixelSize(QSize in_P){
	c_pixelsize_ = in_P;
}
void Camera::SetCenter(QVector3D in_c){
	c_center_ = in_c;
}
void Camera::SetUpVector(QVector3D in_u){
	c_up_ = in_u.normalized();
}
void Camera::SetFocalLength(double in_f){
	c_focallength_ = in_f;
}

Ray* Camera::GetRay(int in_x, int in_y){
	// only get pixel inside the image plane
	if (in_x < -c_clip_.width() / 2 || in_y < -c_clip_.height() / 2 || in_x > c_clip_.width() / 2 || in_y > c_clip_.height() / 2)
	{
		return NULL;
	}

	QVector3D out_o = c_focalpoint_;
	// no anti-aliasing, one pixel with one shotting ray
	QVector3D out_pos = fromPlanetoWorld(in_x, in_y);
	out_pos = c_center_ - out_pos;
	Ray* out_ray = new Ray(out_o, out_pos);
	return out_ray;
}

void Camera::initCamera(){
	// calculate the focal-point
	c_focalpoint_ = c_center_ - c_focallength_ * c_viewdir_;
	// calculate the plane vectors
	c_plane_x_ = c_viewdir_ * c_up_;
	c_plane_y_ = c_plane_x_ * c_viewdir_;
}

QVector3D Camera::fromPlanetoWorld(int in_x, int in_y){
	// x & y represents the pixel coordination, with camera center the origin
	QVector3D out_pos = c_center_ + QVector3D(in_x*c_pixelsize_.width(), in_y*c_pixelsize_.height(), 0);
	if (in_x != 0)
	{
		float pos_t_x = in_x > 0 ? c_pixelsize_.width() / 2 : -c_pixelsize_.width() / 2;
		out_pos.setX(out_pos.x() + pos_t_x);
	}
	if (in_y != 0)
	{
		float pos_t_y = in_y > 0 ? c_pixelsize_.height() / 2 : -c_pixelsize_.height() / 2;
		out_pos.setY(out_pos.x() + pos_t_y);
	}
	return out_pos;
}
