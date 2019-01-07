#include "stdafx.h"
#include <cmath>
#include <algorithm>
#include <cfloat>


//����
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
//����
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
//����
inline void Sort(vector<float>& v)
{
	sort(v.begin(), v.end(), [](const float a, const float b) //�������� ����
	{
		return a < b;
	});
}
////////////////////////////////////////////////////////////////////////
//������
inline float Arithmetic_Mean(vector<float>& v)
{
	float value = 0.f;
	auto size = v.size();
	value = Addition(v) / size;

	if (v.size() == 0) {
		//cout << "��� 0 �߰�" << endl;
	}
	return value;
}

// �������
inline float Geometric_Mean(vector<float>& v)
{
	float reciprocal = 0; //����
	reciprocal = (float)1 / v.size();
	double value = pow(Multiplication(v), reciprocal);

	if (_isnan(value))
		return 0.f;
	else
		return value;
}

// ��ȭ���
inline float Harmonic_Mean(vector<float>& v)
{
	float value = 0;
	float reciprocal = 0; //����
	auto size = v.size();

	for (auto d : v)    //�������� ������
	{
		if (d != 0)
			value += (float)1 / d;
	}
	reciprocal = (float)1 / value;   //�� ����

	if (isinf(reciprocal))
	{
		return 0.f;
	}
	else
		return (size*reciprocal);        //���ϱ� �������� ����
}

//�߾Ӱ�
inline float Median_Mean(vector<float>& v)
{
	Sort(v);

	auto size = v.size();

	int middle_idx = size / 2;

	if (size>=2)
	{
		if (v.size() % 2 == 1)	//Ȧ�����̸� �߰��� ����
			return v[middle_idx];

		else							//¦�����̸� �߰��� �� ���� ���
			return (v[middle_idx] + v[middle_idx - 1]) * 0.5f;
	}
	else return 0.f;
}

//�α����
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

//�������������(�������)
inline float Quadratic_Mean(vector<float>& v)
{
	float value = 0.f;

	for (auto d : v)
	{
		value += d * d;
	}

	if (v.size() == 0) {
		//cout << "���� 0 �߰�" << endl;
	}
	return sqrt(value / v.size());
}

//������ (IQM=midmean)
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
		//cout << "��� 0 �߰�" << endl;
	}
	return value / (size - 2 * quotient);
}