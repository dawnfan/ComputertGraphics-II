#pragma once
#include <QPainter>
#include "raytracer.h"
#include "Camera.h"
#include "SceneRenderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <omp.h>

RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_scenrenderer_ = new SceneRenderer;
	Camera* scene_camera = m_scenrenderer_->SceneCamera();
	m_imagesize_ = scene_camera->Clip();
	m_image_ = new QImage(m_imagesize_, QImage::Format_RGB888);
	m_image_->fill(Qt::black);
	is_antialiasing_ = false;
}

RayTracer::~RayTracer()
{

}


// slots
void RayTracer::CreateImage(){
	//if (m_image_ == NULL)
	//{
	//	m_image_ = new QImage(m_imagesize_, QImage::Format_RGB888);
	//}
	//update();
	RenderImage();
}
void RayTracer::SaveImage(){
	int i, j;
	FILE *fp = fopen("first.ppm", "wb"); /* b - binary mode */
	(void)fprintf(fp, "P6\n%d %d\n255\n", m_imagesize_.width(), m_imagesize_.height());
	for (j = 0; j < m_imagesize_.height(); ++j)
	{
		for (i = 0; i < m_imagesize_.width(); ++i)
		{
			static unsigned char color[3];
			QColor temp_color(m_image_->pixel(i, j));
			color[0] = temp_color.red();  /* red */
			color[1] = temp_color.green();  /* green */
			color[2] = temp_color.blue();  /* blue */
			(void)fwrite(color, 1, 3, fp);
		}
	}
	(void)fclose(fp);
}
void RayTracer::TrigerAntiAliasing(bool is_anti){
	is_antialiasing_ = is_anti;
}

void RayTracer::paintEvent(QPaintEvent *){
	QPainter painter(this);
	painter.begin(this);
	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image 
	if (m_image_ != NULL)
	{
		QRect rect = QRect((width() - m_image_->width()) / 2, (height() - m_image_->height()) / 2, m_image_->width(), m_image_->height());
		painter.drawImage(rect, *m_image_);
	}

	painter.end();
}

void RayTracer::RenderImage(){
	clock_t start_time = clock();
#pragma omp parallel for
	for (int i = 0; i < m_imagesize_.width(); i++)
	{
		for (int j = 0; j < m_imagesize_.height(); j++)
		{
			QVector3D color_vector(0, 0, 0);
			if (is_antialiasing_)
			{

				for (double sample_i = -0.5; sample_i < 0.501; sample_i += 0.5)
				{
					for (double sample_j = -0.5; sample_j < 0.501; sample_j += 0.5)
					{
						color_vector += m_scenrenderer_->GetColor(i + sample_i, j + sample_j);
					}
				}
				color_vector /= 9;
			}
			else
			{
				if (i == 398&& j == 298)
				{
					int x = 0;
				}
				color_vector = m_scenrenderer_->GetColor(i, j);
			}

			m_image_->setPixel(i, j, QColor(color_vector.x(), color_vector.y(), color_vector.z()).rgb());
		}
	}
	update();
	std::cout << (clock() - start_time) / CLOCKS_PER_SEC << std::endl;
}