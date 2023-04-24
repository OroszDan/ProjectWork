#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

class Segment;

class Junction
{
public:

	Junction(int64_t id, float_t lon, float_t lat);

	void AddSegment(Segment* segment);

	~Junction();

	int64_t m_Id;

	float_t m_Lon;

	float_t m_Lat;

private:

	std::vector<Segment*>* m_Segments;
};

struct Node
{
	int64_t m_Id;

	float_t m_Lon;

	float_t m_Lat;
};

