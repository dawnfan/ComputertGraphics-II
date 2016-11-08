#pragma once
#include <vector>
#include <Eigen/Dense>
#include "myvertex.h"

using namespace Eigen;
using namespace std;


class IDW
{
public:
	IDW(vector<MyVertex> &angles);
	~IDW();
	double f_func(MyVertex p);
private:
	vector<MyVertex>* ctrl_angles;
	double u;
	double wi_func(MyVertex p, int i);
	double delta(MyVertex p, int i);
	double distance(MyVertex p, int i);
	double fi_func(MyVertex p, int i);
};