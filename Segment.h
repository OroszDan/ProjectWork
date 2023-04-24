#pragma once

#include "Junction.h"

#include <cmath>
#include <vector>
#include <cstdint>

class Segment
{

public:

	Segment(const int64_t id, const float_t length, const Junction* from, const Junction* to);

	const int64_t m_Id;

	const float_t m_Length;

	const Junction* m_From;

	const Junction* m_To;

};

struct Way
{
	int64_t m_Id;

	float_t m_Length;

	bool m_OneWay;

	int64_t m_IdFrom;

	int64_t m_IdTo;
};

