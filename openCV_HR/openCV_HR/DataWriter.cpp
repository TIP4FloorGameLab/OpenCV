#include "DataWriter.h"


DataBlock::DataBlock(uint64_t _time) : miliseconds(_time)
{
}


DataBlock2::DataBlock2(uint64_t _time, float _x, float _y)
	: DataBlock(_time), x(_x), y(_y)
{
}

CDataWriter::CDataWriter()
{
}


CDataWriter::~CDataWriter()
{

}

void CDataWriter::Append(float _time, float _x, float _y)
{
	mDataList.emplace_back(DataBlock2(_time, _x, _y));
}

void CDataWriter::Append(const DataBlock2& _DataBlock)
{
	mDataList.emplace_back(_DataBlock);
}

void CDataWriter::WriteDataToFile(std::string _filename)
{

	std::ofstream fout;
	fout.open(_filename+".txt");

	for (auto& data : mDataList)
	{
		fout << data.miliseconds << "\t" << data.x << "\t" << data.y << std::endl;
	}
	fout.close();
}


