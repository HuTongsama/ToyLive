#pragma once
#include<string.h>
struct DataHead
{
	enum DataType {
		Null = 0,
		Sound,
		Image,
		Other
	};
	DataType m_type;
	size_t m_dataSize;
	DataHead(DataType type = Null, size_t dataSize = 0)
		:m_type(type), m_dataSize(dataSize) {}
	~DataHead()	{}
};