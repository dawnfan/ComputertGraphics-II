#pragma once

class Ray;

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();
	bool RayHitObject(Ray in_ray);
};

