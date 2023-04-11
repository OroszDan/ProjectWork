#pragma once

#include "Segment.h"
#include "tinyxml2.h"

#include <set>
#include <map>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <json/value.h>
#include <json/writer.h>

class Converter
{
public:

	Converter();

	~Converter();

	void LoadFile(const char* name);

	void SelectNodesNeeded();

	void LoadHighwayNodes();

	void LoadHighways();

	void SaveToJson();

private:

	std::set<int64_t>* m_Node_Ids;
	std::unordered_map<int64_t, Node*>* m_Nodes;
	std::vector<Way*>* m_Ways;
	std::vector<Node*>* m_Route_Node_Ids;

	tinyxml2::XMLDocument m_xDoc;

	tinyxml2::XMLElement* m_Osm; 
	tinyxml2::XMLElement* m_Way;
	tinyxml2::XMLElement* m_Nd;
	tinyxml2::XMLElement* m_Tag;

	float_t CalculateDistanceBetweenTwoLatLonsInMetres(const float_t lat1, const float_t lat2, const float_t lon1, const float_t lon2);

	float_t CalculateLength(const std::vector<Node*>* nodeIds);

	bool IsRoad(const char* roadType);
};

