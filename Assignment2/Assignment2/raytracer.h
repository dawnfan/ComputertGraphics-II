#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QtWidgets/QMainWindow>
#include "ui_raytracer.h"

class QPainter;
class SceneRenderer;

class RayTracer : public QMainWindow
{
	Q_OBJECT

public:
	RayTracer(QWidget *parent = 0);
	~RayTracer();

	// events callback function
	void paintEvent(QPaintEvent *);

private:
	Ui::RayTracerClass	ui;
	
	// my scene renderer
	SceneRenderer*		m_scenrenderer_;
	QImage*				m_image_;
	QSize				m_imagesize_;
	bool				is_antialiasing_;

	// Image operations
	void				RenderImage();
};

#endif // RAYTRACER_H
