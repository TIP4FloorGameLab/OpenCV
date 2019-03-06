#pragma once

#include<iostream>
#include<chrono>
#include<vector>
#include<fstream>
#include<string>

struct DataBlock
{
	uint64_t miliseconds;
	DataBlock(uint64_t _time);
};

// 현재 사용중인 데이터 정보 
struct DataBlock2 : public DataBlock
{
	float x;
	float y;
	DataBlock2(uint64_t _time, float _x, float _y);
};

struct DataBlock3 : public DataBlock2
{
	float z;
};

class CDataWriter {

private:
	std::vector<DataBlock2> mDataList;

public:
	CDataWriter();
	CDataWriter(const CDataWriter& Writer) = delete;
	CDataWriter(CDataWriter&& other) = delete;
	virtual ~CDataWriter();
	
	void Append(float _time, float _x ,float _y);
	void Append(const DataBlock2& _DataBlock);
	void WriteDataToFile(std::string _filename);
};
