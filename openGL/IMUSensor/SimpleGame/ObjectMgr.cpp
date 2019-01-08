#include "stdafx.h"
#include "ObjectMgr.h"

ObjectMgr::ObjectMgr()
{
	// ������ ť���� ��ǥ (���� �����Ǿ� �ȿ�����.)
	fixed_cube.x = 0.f;
	fixed_cube.y = 0.f;
	fixed_cube.z = 0.f;

	fixed_cube.rt_x = 0.f;
	fixed_cube.rt_y = 0.f;
	fixed_cube.rt_z = 0.f;

	// ������ ť���� ��ǥ 
	dynamic_cube.x = 0.f;
	dynamic_cube.y = 0.f + BETWEEN_LENGTH;
	dynamic_cube.z = 0.f;

	dynamic_cube.rt_x = 0.f;
	dynamic_cube.rt_y = 0.f;
	dynamic_cube.rt_z = 0.f;

	renderer_obj = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void ObjectMgr::DrawCube() {
	// ������ ť��
	renderer_obj->DrawSolidCube(fixed_cube.x, fixed_cube.y, fixed_cube.z,
		0.f, 0.f, 0.f, SIZE_OF_CUBE);

	// ������ ť�� 
	//renderer_obj->DrawSolidCube(BETWEEN_LENGTH * sin(theta), BETWEEN_LENGTH * cos(theta), dynamic_cube.z,
	//	0.f, 0.f, -theta, SIZE_OF_CUBE);
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