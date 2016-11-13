#pragma once
#include <QPainter>
#include "raytracer.h"
#include "Camera.h"
#include "SceneRenderer.h"


RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_scenrenderer_ = new SceneRenderer;
	Camera* scene_camera = m_scenrenderer_->SceneCamera();
	m_imagesize_ = scene_camera->Clip();
	m_image_ = new QImage(m_imagesize_, QImage::Format_RGB888);
	is_antialiasing_ = true;
	RenderImage();
}

RayTracer::~RayTracer()
{

}

void RayTracer::paintEvent(QPaintEvent *){
	QPainter painter(this);
	painter.begin(this);
	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image  
	QRect rect = QRect((width() - m_image_->width()) / 2, (height() - m_image_->height()) / 2, m_image_->width(), m_image_->height());
	painter.drawImage(rect, *m_image_);

	painter.end();
}

void RayTracer::RenderImage(){
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
				color_vector = m_scenrenderer_->GetColor(i, j);
			}

			m_image_->setPixel(i, j, QColor(color_vector.x(), color_vector.y(), color_vector.z()).rgb());
		}
	}
}