#pragma once

#include <QVector3D>
#include <QSize>

class Ray;

class Camera
{
public:
	Camera(QSize in_cl = QSize(800, 600),
		QSize in_ps = QSize(8, 6),
		QVector3D in_ct = QVector3D(0, 0, 0),
		QVector3D in_vd = QVector3D(0, 0, 1),
		double in_fl = 8.0,
		QVector3D in_up = QVector3D(0, 1, 0));
	~Camera();

	// set parameters
	void SetClip(QSize in_c);
	void SetView(QVector3D in_v);
	void SetPixelSize(QSize in_P);
	void SetCenter(QVector3D in_c);
	void SetUpVector(QVector3D in_u);
	void SetFocalLength(double in_f);

	// ray operations
	Ray* GetRay(int in_x, int in_y);

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
	QVector3D fromPlanetoWorld(int in_x, int in_y);
};

