/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "SceneMgr.h"
#include "ObjectMgr.h"


SceneMgr* g_SceneMgr = nullptr;
//ObjectMgr* object_mgr = nullptr;

int prev_time = 0;
int tick_time = 0;

vector<ParseNode> buffer;

int ThreadRead()
{
	while (true)
	{
		if (g_SceneMgr->redible) {
			g_SceneMgr->ReadDataAll(buffer);
		}
	}

	return 0;
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 큐브 그리기 
	g_SceneMgr->DrawCube();
	glutSwapBuffers();

	// 연결 안됐을 때 오류 메시지 출력하기 
}


void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{ 
	//키입력에 따라 해당 평균 그래프를 왼쪽에 그려준다.
	switch (key)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Graph");
	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	g_SceneMgr = new SceneMgr;
	g_SceneMgr->SetupSerialPort("100-1", "COM3");

	thread read_thread{ ThreadRead };

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);


	glutMainLoop();
	read_thread.join();

	delete g_SceneMgr;
	return 0;
}
