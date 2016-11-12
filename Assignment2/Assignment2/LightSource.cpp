#include "LightSource.h"

LightSource::LightSource(QVector3D in_ct, QVector3D in_cl):
l_center_(in_ct),
l_color_(in_cl)
{
}


LightSource::~LightSource()
{
}

// set lighting source parameters
void LightSource::SetCenter(QVector3D in_c){
	l_center_ = in_c;
}
void LightSource::SetColor(QVector3D in_c){
	l_color_ = in_c;
}
