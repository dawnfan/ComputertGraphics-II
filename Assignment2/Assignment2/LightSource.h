#pragma once

#include <QVector3D>

// point lighting source
class LightSource
{
public:
	LightSource(QVector3D in_ct = QVector3D(0, 200, 500), QVector3D in_cl = QVector3D(255, 255, 255));
	~LightSource();

	// set lighting source parameters
	void SetCenter(QVector3D in_c);
	void SetColor(QVector3D in_c);

	// obtain lighting source parameters
	QVector3D LightCenter(){ return l_center_; };
	QVector3D LightColor(){ return l_color_; };

private:
	QVector3D		l_center_;
	QVector3D		l_color_;
};

