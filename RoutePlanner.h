#pragma once

#include "Converter.h"

class RoutePlanner
{

public:
	RoutePlanner();
	void Initialize();
	void Search(const int64_t from, const int64_t to);
	Junction* GetMin(std::shared_ptr<std::unordered_map<int64_t, int64_t>> S, std::shared_ptr<std::unordered_map<int64_t, Junction*>> LE);
	float_t GetHeuristicDistance(const Junction* start, const Junction* target);

private:

	std::shared_ptr<std::unordered_map<int64_t, Junction*>> m_Junctions;
	std::shared_ptr<std::vector<Segment*>> m_Segments;
};

