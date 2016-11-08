#pragma once

#include <vector>

// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM Mathematics
#include <glm/glm.hpp>
// Triangulation
#include "triangle.h"
#include "IDW.h"


class bezier
{
private:
	// math:C(n,i)
	int* binomial_coefficient_;
	// number of control points in a row/column (row == colomn)
	int control_length_;
	// step to sample u and v
	float sample_step_;

	std::vector<MyVertex>* control_points_;
	std::vector<MyVertex>* bezier_points_;
	int* triangulation_edges_;
	int edges_num_;
	int* ctrl_edges_;
	int ctrl_edges_num_;

	// adaptive sampling
	void calcCtrlAngles(std::vector<MyVertex>* ctrl_angles);
	

	void initCtrlPoints(); // initialize control points (control_length*control_length)
	void setCtrlPoints();
	void initBinomialCoefficient(); // get n! (n <= control_length)
	void initSamplePoints(); // initialize sample points with sample_step
	// p(u, v) = sum(sum(B(u)_n_i * B(v)_m_j * k(i, j)))
	// B(t)_n_i = C(n, i) * u^i * (1-u)^(n-i)
	float calculateBernstein(float t,int i); // calculate B(t)_n_i
	MyVertex calculateBezierPointUV(float u_in, float v_in); // calculate B(u)_n_i * B(v)_m_j * k(i, j)
	void calculateBezierPoints(); // calculate p(u, v)
	void calculateEdges(std::vector<MyVertex>* points_in_, bool isCtrl); // get point indices of faces

	// operations for MyVertex
		// calculate multiplication of a vertex and a number
	void scalarMultVertex(MyVertex& v_in, float s, MyVertex* v_out); 
	float distanceVertex(MyVertex& v_in);

public:
	bezier(){
		control_length_ = 4;
		sample_step_ = 0.1f;
		initBinomialCoefficient();
		initCtrlPoints();
		calculateEdges(control_points_, true);
		initSamplePoints();
		calculateEdges(bezier_points_, false);

		calculateBezierPoints();
	};
	~bezier();

	std::vector<MyVertex>* getBezierPoints(){ return bezier_points_; };
	std::vector<MyVertex>* getControlPoints(){ return control_points_; };
	int* getBezierEdges(){ return triangulation_edges_; };
	int* getCtrlEdges(){ return ctrl_edges_; };
	int getEdgesNum(){ return edges_num_; };
	int getCtrlEdgesNum(){ return ctrl_edges_num_; };
};

