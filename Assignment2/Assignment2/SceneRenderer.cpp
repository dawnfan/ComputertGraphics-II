#pragma once
#include "SceneRenderer.h"
#include "Camera.h"
#include "Ray.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "LightSource.h"

SceneRenderer::SceneRenderer()
{
}


SceneRenderer::~SceneRenderer()
{
}

bool SceneRenderer::RayHitObject(Ray in_ray){

	return false;
}