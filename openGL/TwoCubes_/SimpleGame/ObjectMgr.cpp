#include "stdafx.h"
#include "ObjectMgr.h"

ObjectMgr::ObjectMgr()
{
	// 고정된 큐브의 좌표 (정말 고정되서 안움직임.)
	fixed_cube.x = 0.f;
	fixed_cube.y = 0.f;
	fixed_cube.z = 0.f;

	fixed_cube.rt_x = 0.f;
	fixed_cube.rt_y = 0.f;
	fixed_cube.rt_z = 0.f;

	// 동적인 큐브의 좌표 
	dynamic_cube.x = 0.f;
	dynamic_cube.y = 0.f + BETWEEN_LENGTH;
	dynamic_cube.z = 0.f;

	dynamic_cube.rt_x = 0.f;
	dynamic_cube.rt_y = 0.f;
	dynamic_cube.rt_z = 0.f;

	renderer_obj = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void ObjectMgr::DrawCube() {
	// 고정된 큐브

	renderer_obj->DrawSolidCube(fixed_cube.x, fixed_cube.y, fixed_cube.z,
		0.f, 0.f, 0.f, SIZE_OF_CUBE);
	// 동적인 큐브 
	renderer_obj->DrawSolidCube(BETWEEN_LENGTH * sin(theta), BETWEEN_LENGTH * cos(theta), dynamic_cube.z,
		0.f, 0.f, -theta, SIZE_OF_CUBE);

	
}

void ObjectMgr::MoveTick() {
	//dynamic_cube.rt_z += 20.f;
	//dynamic_cube.rt_y += 20.f;
	//dynamic_cube.rt_x += 20.f;
	theta += 0.1f;
}

ObjectMgr::~ObjectMgr()
{
	delete renderer_obj;
}

void Rotate(float angle, float x, float y, float z) {

}