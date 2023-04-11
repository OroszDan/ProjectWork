#pragma once

#include "Junction.h"

#include <cmath>
#include <vector>
#include <cstdint>

class Segment
{

public:

	Segment();

	uint8_t m_Maxspeed;

	int32_t m_Length;

	Junction* m_From;

	Junction* m_To;

};

struct Way
{
	int64_t m_Id;

	float_t m_Length;

	bool m_OneWay;

	int64_t m_IdFrom;

	int64_t m_IdTo;
};

