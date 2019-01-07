#pragma once
#include "Renderer.h"


class ObjectMgr
{
	Vec fixed_cube;
	Vec dynamic_cube;
	glm::mat4 move_mat;
	Renderer* renderer_obj = nullptr;
	float theta = 0.f;
public:
	ObjectMgr();
	~ObjectMgr();
	void DrawCube();
	void MoveTick();

	void Rotate(float angle, float x, float y, float z);
};

