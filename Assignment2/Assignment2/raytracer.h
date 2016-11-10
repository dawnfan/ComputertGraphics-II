#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QtWidgets/QMainWindow>
#include "ui_raytracer.h"

class RayTracer : public QMainWindow
{
	Q_OBJECT

public:
	RayTracer(QWidget *parent = 0);
	~RayTracer();

private:
	Ui::RayTracerClass ui;
};

#endif // RAYTRACER_H
