#pragma once
#include "Ray.h"
#include "Camera.h"
#include "LightSource.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "SceneRenderer.h"


#define MAX_FLOAT_VALUE (static_cast<float>(10e10))
#define MIN_FLOAT_VALUE	(static_cast<float>(-10e10))

SceneRenderer::SceneRenderer()
{
	initScene();
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::initScene(){
	// new the things in the scene
	s_amibentcolor_ = QVector3D(200, 200, 200);
	s_camera_ = new Camera();
	s_camera_->SetClip(QSize(800, 600));
	s_camera_->SetFocalLength(1000);
	s_light_list_ = new QVector < LightSource* > ;
	s_object_list_ = new QVector < AbObject* > ;
	// set the camera
	// create the lights
	s_light_list_->push_back(new LightSource(QVector3D(0, 500, 100)));
	// create the objects
	AbObject* plane_1 = new PlaneObject(QVector3D(0, 0, 2000), QVector3D(0, 0, -1), QVector3D(200, 178, 204));
	s_object_list_->push_back(plane_1);
	AbObject* plane_2 = new PlaneObject(QVector3D(1500, 0, 100), QVector3D(-1, 0, 0), QVector3D(140, 170, 200));
	s_object_list_->push_back(plane_2);
	AbObject* plane_3 = new PlaneObject(QVector3D(-1500, 0, 100), QVector3D(1, 0, 0), QVector3D(110, 170, 110));
	s_object_list_->push_back(plane_3);
	AbObject* plane_4 = new PlaneObject(QVector3D(0, -800, 100), QVector3D(0, 1, 0), QVector3D(120, 120, 120));
	s_object_list_->push_back(plane_4);
	AbObject* plane_5 = new PlaneObject(QVector3D(0, 2000, 100), QVector3D(0, -1, 0), QVector3D(120, 120, 120));
	s_object_list_->push_back(plane_5);
	AbObject* sphere_1 = new SphereObject(QVector3D(800, -400, 1000));
	s_object_list_->push_back(sphere_1);
	AbObject* sphere_2 = new SphereObject(QVector3D(-300, -100, 500), 200, QVector3D(145, 231, 235));
	s_object_list_->push_back(sphere_2);
}

bool SceneRenderer::RayHitObject(Ray in_ray, IntersectPoint& out_point){
	bool ishit = false;
	QVector3D ray_origin = in_ray.OriginPoint();
	double out_dist = MAX_FLOAT_VALUE;
	IntersectPoint temp_out;
	// check each object for intersection, and find the closed
	for (int ob_num = 0; ob_num < s_object_list_->size(); ob_num++)
	{
		AbObject* cur_ob = s_object_list_->at(ob_num);
		if (cur_ob->RayHitTest(in_ray, temp_out))
		{
			ishit = true;
			double temp_dist = (temp_out.out_point - ray_origin).length();
			if (temp_dist < out_dist)
			{
				out_dist = temp_dist;
				out_point = temp_out;
			}
		}
	}
	// check the light
	if (s_light_list_->at(0)->RayHitTest(in_ray))
	{
		double dist = (s_light_list_->at(0)->LightCenter() - in_ray.OriginPoint()).length();
		if (dist < out_dist)
		{
			out_point.isLight = true;
		}
	}

	return ishit;
}


// rendering operations
QVector3D SceneRenderer::GetColor(double in_x, double in_y){
	QVector3D out_color(0, 0, 0);
	Ray* in_ray = s_camera_->GetRay(in_x, in_y);
	IntersectPoint hit_point;
	// when hit one object, set the color
	bool ishit = RayHitObject((*in_ray), hit_point);
	if (ishit)
	{
		if (hit_point.isLight)
		{
			out_color = QVector3D(255, 255, 255);
		}
		else
		{
			// todo: add Phong model
			QVector3D light_dir = s_light_list_->at(0)->LightCenter() - hit_point.out_point;
			Ray shadow_ray(hit_point.out_point, light_dir);
			IntersectPoint temp_p;
			bool is_shadow = RayHitObject(shadow_ray, temp_p);
			if (!is_shadow || temp_p.isLight)
			{
				light_dir.normalize();
				float diff_ratio = QVector3D::dotProduct(light_dir, hit_point.out_normal);
				//out_color += 0.2 * s_amibentcolor_;
				out_color += 0.1 * hit_point.out_color;
				if (diff_ratio > 1e-2)
				{
					out_color += diff_ratio * hit_point.out_color;
				}
				validColor(out_color);
			}
		}
	}

	return out_color;
}

void SceneRenderer::validColor(QVector3D& in_color){
	double temp_value = 0.0;
	temp_value = in_color.x();
	temp_value = temp_value > 255 ? 255 : temp_value;
	temp_value = temp_value < 0 ? 0 : temp_value;
	in_color.setX(temp_value);
	temp_value = in_color.y();
	temp_value = temp_value > 255 ? 255 : temp_value;
	temp_value = temp_value < 0 ? 0 : temp_value;
	in_color.setY(temp_value);
	temp_value = in_color.z();
	temp_value = temp_value > 255 ? 255 : temp_value;
	temp_value = temp_value < 0 ? 0 : temp_value;
	in_color.setZ(temp_value);
}

bool SceneRenderer::isShadow(){
	bool isshadow = false;

	return isshadow;
}