#pragma once

#include <cmath>
#include <cstdint>

class Junction
{
public:

	Junction(int64_t id, float_t lon, float_t lat);

	int64_t m_Id;

	float_t m_Lon;

	float_t m_Lat;

};

struct Node
{
	int64_t m_Id;

	float_t m_Lon;

	float_t m_Lat;
};

