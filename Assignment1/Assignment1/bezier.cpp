#include "bezier.h"
#include <math.h>

#define PI 3.14159265

bezier::~bezier()
{
	//delete []binomial_coefficient_;
	control_points_->clear();
	delete control_points_;
	bezier_points_->clear();
	delete bezier_points_;
}

void bezier::initCtrlPoints(){
	/*for (int i = 0; i < control_length_; i ++)
	{
	for (int j = 0; j < control_length_; j ++)
	{
	control_points_.push_back(glm::vec3());
	}
	}*/
	control_points_ = new std::vector < MyVertex > ;
	for (int i = 0; i < control_length_; i++)
	{
		for (int j = 0; j < control_length_; j++)
		{
			MyVertex temp_p = { 0.0f+i, 0.0f+j, 0.0f };
			control_points_->push_back(temp_p);
		}
	}
	return;
}

void bezier::initBinomialCoefficient(){
	int* temp_value = new int(control_length_);
	// C(n,0) = 1
	temp_value[0] = 1;
	// count factorial
	int temp_temp_value = 1;
	for (int i = 1; i < control_length_; i++)
	{
		temp_value[i] = temp_temp_value*i;
	}

	// count Binomial Coefficient
	binomial_coefficient_ = new int(control_length_);
	binomial_coefficient_[0] = 1;
	for (int i = 1; i < control_length_; i++)
	{
		binomial_coefficient_[i] = temp_value[control_length_-1] / temp_value[i] / temp_value[control_length_-1 - i];
	}

	//delete[]temp_value;
}


void bezier::initSamplePoints(){
	setCtrlPoints();
	bezier_points_ = new std::vector < MyVertex >;
	float limit_step = 1.0f + sample_step_;
	//for (float u_step = 0.0f; u_step < limit_step; u_step += sample_step_)
	//{
	//	for (float v_step = 0.0f; v_step < limit_step; v_step += sample_step_){
	//		MyVertex temp_p = { u_step, v_step, 0.0f };
	//		bezier_points_->push_back(temp_p);
	//	}
	//}
	std::vector<MyVertex> my_angles;
	calcCtrlAngles(&my_angles);
	IDW my_idw(my_angles);
	for (float u_step = 0.0f; u_step < limit_step; u_step += sample_step_)
	{
		for (float v_step = 0.0f; v_step < limit_step; v_step += sample_step_){
			MyVertex temp_p = { u_step, v_step, 0.0f };
			double ratio_sample = my_idw.f_func(temp_p)/PI;
			temp_p.x = temp_p.x*ratio_sample;
			temp_p.y = temp_p.y*ratio_sample;
			bezier_points_->push_back(temp_p);
			if (u_step + sample_step_ <limit_step)
			{
				bezier_points_->push_back(MyVertex{ u_step + sample_step_, v_step, 0.0f });
			}
			if (v_step + sample_step_ < limit_step)
			{
				bezier_points_->push_back(MyVertex{ u_step, v_step + sample_step_, 0.0f });
			}
		}
		if (u_step > 0.9f)
		{
			int x = 0;
		}
	}
}

void bezier::calcCtrlAngles(std::vector<MyVertex>* ctrl_angles){
	//(acos(cosinNormal) / PI * 180.0)
	float* length_sum_x = new float(control_length_);
	float* length_sum_y = new float(control_length_);
	for (int i = 1; i < control_length_-1; i++)
	{
		for (int j = 1; j < control_length_-1; j++)
		{
			MyVertex my_v = { 0.0f, 0.0f, 0.0f };
			MyVertex* cur_v = &(control_points_->at((i)*(control_length_)+j));
			MyVertex v_a = { cur_v->x, cur_v->y, cur_v->z }, v_b = { cur_v->x, cur_v->y, cur_v->z };
			scalarMultVertex(control_points_->at((i - 1)*(control_length_)+j), -1.0f, &v_a);
			scalarMultVertex(control_points_->at((i + 1)*(control_length_)+j), -1.0f, &v_b);
			length_sum_x[i-1] += distanceVertex(v_a);
			
			my_v.x = distanceVertex(v_a);
			float dis_x = my_v.x*distanceVertex(v_b);
			dis_x = (v_a.x*v_b.x + v_a.y*v_b.y + v_a.z*v_b.z)/dis_x;
		
			v_a = { cur_v->x, cur_v->y, cur_v->z };
			v_b = { cur_v->x, cur_v->y, cur_v->z };
			scalarMultVertex(control_points_->at(i*(control_length_)+(j-1)), -1.0f, &v_a);
			scalarMultVertex(control_points_->at(i*(control_length_)+(j+1)), -1.0f, &v_b);
			length_sum_y[i-1] += distanceVertex(v_a);
			my_v.y = distanceVertex(v_a);
			float dis_y = my_v.y*distanceVertex(v_b);
			dis_y = (v_a.x*v_b.x + v_a.y*v_b.y + v_a.z*v_b.z) / dis_y;


			dis_x = acos(dis_x);
			dis_y = acos(dis_y);

			my_v.z = (dis_x + dis_y) / 2.0f;
			ctrl_angles->push_back(my_v);
		}
	}
	for (int i = 1; i < control_length_ - 1; i++)
	{
		for (int j = 1; j < control_length_ - 1; j++)
		{
			MyVertex* temp_v = &(ctrl_angles->at((i - 1)*(control_length_ - 2) + j-1));
			temp_v->x = temp_v->x / length_sum_x[i - 1];
			temp_v->y = temp_v->y / length_sum_y[j - 1];
		}
	}
}

float bezier::calculateBernstein(float t, int i){
	float b_out = 1.0f;
	float t_conv = 1.0f - t;
	// B(t)^i = C(n,i)*t^i*(1-t)^(n-i)
	b_out *= (float)binomial_coefficient_[i];
	b_out *= pow(t, i);
	b_out *= pow(t_conv, control_length_-1-i);

	return b_out;
}

MyVertex bezier::calculateBezierPointUV(float u_in, float v_in){
	MyVertex bezier_point_uv = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < control_length_; i++)
	{
		for (int j = 0; j < control_length_; j++)
		{
			float ratio_bernstein = calculateBernstein(u_in, i)*calculateBernstein(v_in, j);
			scalarMultVertex(control_points_->at(i*(control_length_) + j), ratio_bernstein, &bezier_point_uv);
		}
	}
	return bezier_point_uv;
}

void bezier::calculateBezierPoints(){
	//bezier_points_ = new std::vector < MyVertex > ;
	int num_ = bezier_points_->size();
	for (int i = 0; i < num_; i++)
	{
		MyVertex temp = calculateBezierPointUV(bezier_points_->at(i).x, bezier_points_->at(i).y);
		bezier_points_->at(i) = temp;
	}
	//for (float u_step = 0.0f; u_step < 1.0f+sample_step_; u_step += sample_step_)
	//{
	//	for (float v_step = 0.0f; v_step < 1.0f+sample_step_; v_step += sample_step_)
	//	{
	//		MyVertex temp = calculateBezierPointUV(u_step, v_step);
	//		bezier_points_->push_back(temp);
	//	}
	//}
}

void bezier::setCtrlPoints(){
	// control points
	control_points_->clear();
	//delete[] control_points_;
	MyVertex temp_p[] = {
		-1.0f, 0.0f, 1.0f,
		-0.5f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		-0.5f, 1.0f, 0.5f,

		-1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.5f,

		-1.0f, 0.0f, -1.0f,
		-0.5f, 1.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		-0.5f,0.5f,0.5f,

		-0.5f,0.0f,-0.5f,
		0.0f,1.0f,-0.5f,
		0.5f,0.5f,-1.0f,
		0.0f,0.5f,0.5f,

		0.0f, 0.0f, 0.0f
	};
	control_points_->insert(control_points_->begin(), &temp_p[0], &temp_p[control_length_*control_length_]);
	//control_points_ = new std::vector<MyVertex>(&temp_p[0], &temp_p[control_length_*control_length_ - 1]);
}

void bezier::calculateEdges(std::vector<MyVertex>* points_in_, bool isCtrl){
	points_in_->erase(points_in_->begin());
	struct triangulateio t_in, t_out, vorout;
	t_in.numberofpointattributes = 0;
	t_in.numberofpoints = points_in_->size();
	t_in.pointlist = (REAL *)malloc(t_in.numberofpoints * 2 * sizeof(REAL));
	for (int i = 0; i < t_in.numberofpoints; i++)
	{
		t_in.pointlist[2 * i] = points_in_->at(i).x;
		t_in.pointlist[2 * i + 1] = points_in_->at(i).y;
	}
	t_in.pointmarkerlist = NULL;

	t_out.numberoftriangleattributes = 0;
	t_out.trianglelist = NULL;
	triangulate("zNBQ", &t_in, &t_out, &vorout);
	if (isCtrl)
	{
		ctrl_edges_ = t_out.trianglelist;
		ctrl_edges_num_ = t_out.numberoftriangles;
	}
	else{
		triangulation_edges_ = t_out.trianglelist;
		edges_num_ = t_out.numberoftriangles;
	}
}

void bezier::scalarMultVertex(MyVertex& v_in, float s, MyVertex* v_out){
	v_out->x += v_in.x*s;
	v_out->y += v_in.y*s;
	v_out->z += v_in.z*s;
}

float bezier::distanceVertex(MyVertex& v_in){
	float dis = 0.0f;
	dis += pow(v_in.x, 2);
	dis += pow(v_in.y, 2);
	dis += pow(v_in.z, 2);
	dis = sqrtf(dis);
	return dis;
}