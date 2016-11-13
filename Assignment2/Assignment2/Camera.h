#pragma once

#include <QVector3D>
#include <QSize>

class Ray;

class Camera
{
public:
	Camera(QSize in_cl = QSize(400, 300),
		QSize in_ps = QSize(3, 3),
		QVector3D in_ct = QVector3D(0, 0, 0),
		QVector3D in_vd = QVector3D(0, 0, 1),
		double in_fl = 500.0,
		QVector3D in_up = QVector3D(0, 1, 0));
	~Camera();

	// set parameters
	void SetClip(QSize in_c);
	void SetView(QVector3D in_v);
	void SetPixelSize(QSize in_P);
	void SetCenter(QVector3D in_c);
	void SetUpVector(QVector3D in_u);
	void SetFocalLength(double in_f);
	// obtain parameters
	QSize Clip(){ return c_clip_; };
	QVector3D ViewVector(){ return c_viewdir_; };
	QSize PixelSize(){ return c_pixelsize_; };
	QVector3D CameraCenter(){ return c_center_; };
	QVector3D UpVector(){ return c_up_; };
	double FocalLength(){ return c_focallength_; };

	// ray operations
	Ray* GetRay(double in_x, double in_y);

private:
	QVector3D		c_up_;	// up vector
	QSize			c_clip_; // the clip to determine the view-port
	QVector3D		c_center_; // camera center, look-at
	QVector3D		c_viewdir_; // view vector
	QSize			c_pixelsize_;
	double			c_focallength_;

	QVector3D		c_focalpoint_; // the focal point, calculated by focal-length & camera center & view vector
	QVector3D		c_plane_x_; // the image plane coordination axis
	QVector3D		c_plane_y_;

	void initCamera();
	QVector3D fromPlanetoWorld(double in_x, double in_y);
};

