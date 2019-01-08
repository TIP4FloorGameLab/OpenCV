#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <random>
#include <sstream>
#include <thread>
#include <Windows.h>

using namespace std;

typedef struct Vec {
	float x;
	float y;
	float z;

	// Rotation Degree
	float rt_x;
	float rt_y;
	float rt_z;
}Vec;

struct MyMatrix {
	float x0, y0, z0;
	float x1, y1, z1;
	float x2, y2, z2;
};



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//-------------------------------------------------------------------------------
#define NUM_OF_CUBE	2
#define BETWEEN_LENGTH	100.f
#define SIZE_OF_CUBE	50.f
#define RADIAN			180.f / 3.141592
//-------------------------------------------------------------------------------


#define deltaTime 1.f

#define OP_GYRO_X 0
#define OP_GYRO_Y 1
#define OP_GYRO_Z 2

#define OP_ACC_X 3
#define OP_ACC_Y 4
#define OP_ACC_Z 5


// 가중치 
#define WEIGHT_A	0.2f
#define WEIGHT_G	0.2f
#define WEIGHT_H	0.2f
#define WEIGHT_M 0.2f
#define WEIGHT_L	0.f
#define WEIGHT_Q	0.f
#define WEIGHT_I	0.2f


static float PI = 3.141592;

const float LEVEL_RECT = 0.1f;

using INFO = pair<float, float>;  // XCoord, Mean value

typedef struct ParseNode {		//센서에서 받는 정보들
	string str_id;
	float euler_x;
	float euler_y;
	float euler_z;
	//float gyro_x;
	//float gyro_y;
	//float gyro_z;
	//float acc_x;
	//float acc_y;
	//float acc_z;
	float distance_x;
	float distance_y;
	float distance_z;

	int battery;
} ParseNode;

enum MEAN		//평균을 내는 함수의 종류
{
	Weighted,
	Arithmetic,
	Geometric,
	Harmonic,
	Median,
	Log,
	Quadratic,
	Interquartile
};

typedef struct Point  //평균값에 해당되는 점
{
	float Apoint[3];	//산술평균
	float Gpoint[3];  //기하평균
	float Hpoint[3];  //조화평균
	float Mpoint[3]; //중앙값
	float Lpoint[3];  //로그평균
	float Qpoint[3]; //이차평균
	float Ipoint[3];  //사분평균

} Point;


