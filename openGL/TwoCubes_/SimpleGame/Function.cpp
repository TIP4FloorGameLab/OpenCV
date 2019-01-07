#include "stdafx.h"
#include <cmath>
#include <algorithm>
#include <cfloat>


//덧셈
inline float Addition(vector<float>& const v)
{
	float value = 0;
	auto size = v.size();

	for (auto d : v)
	{
		value += d;
	}

	return value;
}
//곱셈
inline float Multiplication(vector<float>& v)
{
	float value = 1;
	auto size = v.size();

	for (auto d : v)
	{
		value *= d;
	}

	return value;
}
//정렬
inline void Sort(vector<float>& v)
{
	sort(v.begin(), v.end(), [](const float a, const float b) //오름차순 정렬
	{
		return a < b;
	});
}
////////////////////////////////////////////////////////////////////////
//산술평균
inline float Arithmetic_Mean(vector<float>& v)
{
	float value = 0.f;
	auto size = v.size();
	value = Addition(v) / size;

	if (v.size() == 0) {
		//cout << "산술 0 발견" << endl;
	}
	return value;
}

// 기하평균
inline float Geometric_Mean(vector<float>& v)
{
	float reciprocal = 0; //역수
	reciprocal = (float)1 / v.size();
	double value = pow(Multiplication(v), reciprocal);

	if (_isnan(value))
		return 0.f;
	else
		return value;
}

// 조화평균
inline float Harmonic_Mean(vector<float>& v)
{
	float value = 0;
	float reciprocal = 0; //역수
	auto size = v.size();

	for (auto d : v)    //역수들의 산술평균
	{
		if (d != 0)
			value += (float)1 / d;
	}
	reciprocal = (float)1 / value;   //의 역수

	if (isinf(reciprocal))
	{
		return 0.f;
	}
	else
		return (size*reciprocal);        //곱하기 데이터의 개수
}

//중앙값
inline float Median_Mean(vector<float>& v)
{
	Sort(v);

	auto size = v.size();

	int middle_idx = size / 2;

	if (size>=2)
	{
		if (v.size() % 2 == 1)	//홀수개이면 중간값 리턴
			return v[middle_idx];

		else							//짝수개이면 중간값 앞 뒤의 평균
			return (v[middle_idx] + v[middle_idx - 1]) * 0.5f;
	}
	else return 0.f;
}

//로그평균
inline float Log_Mean(vector<float>& v)
{
	float value = 0.f;
	float v1(0.f), v2(0.f);

	if (v.size() >= 2)
	{
		auto i = v.begin();
		{
			value = abs((*i) - *(i + 1)) / abs(log(*i) - log(*(i + 1)));
		}
	}
	if (_isnan(value))
		return 0.f;
	else
		return value;
}

//제곱평균제곱근(이차평균)
inline float Quadratic_Mean(vector<float>& v)
{
	float value = 0.f;

	for (auto d : v)
	{
		value += d * d;
	}

	if (v.size() == 0) {
		//cout << "이차 0 발견" << endl;
	}
	return sqrt(value / v.size());
}

//사분평균 (IQM=midmean)
inline float  Interquartile_Mean(vector<float>&v)
{
	Sort(v);

	float value = 0.f;
	auto size = v.size();
	int quotient = size / 4;

	for (auto i = v.begin() + quotient; i != v.end() - quotient; ++i)
	{
		value += *i;
	}

	if (v.size() == 0) {
		//cout << "사분 0 발견" << endl;
	}
	return value / (size - 2 * quotient);
}