#include "IDW.h"
#include <math.h>

IDW::IDW(vector<MyVertex> &angles)
{
	u = 2.0;
	ctrl_angles = &angles;
}

IDW::~IDW()
{

}

double IDW::wi_func(MyVertex p, int i)
{
	double tmp_sum = 0;
	double result;

	if (p.x == ctrl_angles->at(i).x &&
		p.y == ctrl_angles->at(i).y)
		return 1;//exactly the control points

	for (size_t j = 0; j < ctrl_angles->size(); j++)
	{
		tmp_sum += delta(p, j);
	}

	result = delta(p, i) / tmp_sum;
	return result;
}

double IDW::distance(MyVertex p, int i)
{
	double m_result;

	//暂时不开方
	m_result = (p.x - ctrl_angles->at(i).x) *
		(p.x - ctrl_angles->at(i).x) +
		(p.y - ctrl_angles->at(i).y) *
		(p.y - ctrl_angles->at(i).y);
	return m_result;
}

double IDW::delta(MyVertex p, int i)
{
	double m_result;

	m_result = pow(1.0 / distance(p, i), u / 2.0);
	//with distance^2,u should be divided by 2.0
	return m_result;
}

double IDW::fi_func(MyVertex p, int i)
{
	double result = 0.0;

	result = ctrl_angles->at(i).z;

	return result;
}

double IDW::f_func(MyVertex p)
{
	double result = 0.0;

	for (size_t i = 0; i < ctrl_angles->size(); i++)
	{
		result += fi_func(p, i) * wi_func(p, i);
	}

	return result;
}