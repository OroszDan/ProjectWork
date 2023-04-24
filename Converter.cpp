#include "Converter.h"

#include <string>
#include <cmath>
#include <numbers>
#include <tinyxml2.h>
#include <json/value.h>
#include <json/writer.h>
#include <json/reader.h>

const uint32_t earthRadius = 6371000;  //in metres

Converter::Converter()
{
	m_Node_Ids = new std::set<int64_t>();
	m_Nodes = new std::unordered_map<int64_t, Node*>();
	m_Ways = new std::vector<Way*>();
	m_Route_Node_Ids = new std::vector<Node*>();

	m_Osm = nullptr;
	m_Way = nullptr;
	m_Nd = nullptr;
	m_Tag = nullptr;

}

Converter::~Converter()
{
	m_Node_Ids = nullptr;
	m_Nodes = nullptr;
	m_Ways = nullptr;
	m_Route_Node_Ids = nullptr;

	m_Osm = nullptr;
	m_Way = nullptr;
	m_Nd = nullptr;
	m_Tag = nullptr;
}

void Converter::LoadJsonFile(const char* fileName, Json::Value& root)
{
	std::ifstream inputStream(fileName);
	Json::Reader reader;

	bool success = reader.parse(inputStream, root);

	if (!success)
	{
		throw new std::exception("Json file not loaded correctly");
	}
}

void Converter::LoadOsmFile(const char* name)
{
	auto err = m_xDoc.LoadFile(name);

	if (err != tinyxml2::XML_SUCCESS)
	{
		throw new std::exception("Osm file not loaded correctly");
	}

	m_Osm = m_xDoc.FirstChildElement("osm");

}

void Converter::GetPreprocessedData(const Json::Value& root, std::unordered_map<int64_t, Junction*>* Junctions, std::vector<Segment*>* Segments)
{	
	Junction* junction;

	for (auto it = root["nodes"].begin(); it != root["nodes"].end(); ++it) 
	{
		Json::Value node = it->get("node", Json::nullValue);
		
		if (node != Json::nullValue)
		{
			int64_t id = node["id"].asInt64();
			float_t lat = node["lat"].asFloat();
			float_t lon = node["lon"].asFloat();
			
			junction = new Junction(id, lon, lat);
			Junctions->insert(std::make_pair(junction->m_Id, junction));
		}	
	}

	Segment* segment;

	for (auto it = root["ways"].begin(); it != root["ways"].end(); ++it)
	{
		Json::Value node = it->get("way", Json::nullValue);

		if (node != Json::nullValue)
		{
			int64_t id = node["id"].asInt64();
			int64_t idFrom = node["idfrom"].asInt64();
			int64_t idTo = node["idto"].asInt64();
			float_t length = node["length"].asFloat();
			bool oneWay = node["oneway"].asBool();

			segment = new Segment(id, length, Junctions->at(idFrom), Junctions->at(idTo));

			if (oneWay)
			{
				Junctions->at(idFrom)->AddSegment(segment);
			}
			else
			{
				Junctions->at(idFrom)->AddSegment(segment);
				Junctions->at(idTo)->AddSegment(segment);
			}
			
			Segments->push_back(segment);
		}
	}
}

void Converter::SelectNodesNeeded()
{
	m_Node_Ids = new std::set<int64_t>();

	m_Way = m_Osm->FirstChildElement("way");

	const tinyxml2::XMLAttribute* id;

	while (m_Way != nullptr)
	{
		m_Tag = m_Way->FirstChildElement("tag");
		auto Iid = m_Way->FindAttribute("id")->Int64Value();

		while (m_Tag != nullptr && strcmp(m_Tag->FindAttribute("k")->Value(), "highway"))
		{
			m_Tag = m_Tag->NextSiblingElement("tag");
		}

		if (m_Tag != nullptr)
		{
			auto roadType = m_Tag->FindAttribute("v")->Value();
			if (IsRoad(roadType))
			{

				m_Nd = m_Way->FirstChildElement("nd");
				while (m_Nd != nullptr)
				{
					auto ref = m_Nd->FindAttribute("ref")->Int64Value();

					m_Node_Ids->insert(ref);
					m_Nd = m_Nd->NextSiblingElement("nd");
				}

			}

		}

		m_Way = m_Way->NextSiblingElement("way");
	}
}

void Converter::LoadHighwayNodes()
{
	const tinyxml2::XMLAttribute* lat;
	const tinyxml2::XMLAttribute* lon;
	const tinyxml2::XMLAttribute* id;

	tinyxml2::XMLElement* node;

	m_Nodes = new std::unordered_map<int64_t, Node*>();

	node = m_Osm->FirstChildElement("node");

	int size = 0;

	while (node != nullptr)
	{
		id = node->FindAttribute("id");

		if (std::find(m_Node_Ids->cbegin(), m_Node_Ids->cend(), id->Int64Value()) != m_Node_Ids->cend())
		{
			lat = node->FindAttribute("lat");
			lon = node->FindAttribute("lon");

			Node* highway_node = new Node();
			highway_node->m_Id = id->Int64Value();
			highway_node->m_Lat = lat->float_tValue();
			highway_node->m_Lon = lon->float_tValue();

			m_Nodes->insert(std::make_pair(highway_node->m_Id, highway_node));

		}

		node = node->NextSiblingElement("node");
	}
}

void Converter::LoadHighways()
{
	m_Ways = new std::vector<Way*>();

	const tinyxml2::XMLAttribute* id;

	m_Way = m_Osm->FirstChildElement("way");

	while (m_Way != nullptr)
	{
		m_Tag = m_Way->FirstChildElement("tag");
		auto Iid = m_Way->FindAttribute("id")->Int64Value();

		while (m_Tag != nullptr && strcmp(m_Tag->FindAttribute("k")->Value(), "highway"))
		{
			m_Tag = m_Tag->NextSiblingElement("tag");
		}

		if (m_Tag != nullptr)
		{
			m_Route_Node_Ids = new std::vector<Node*>();
			id = m_Way->FindAttribute("id");
			m_Nd = m_Way->FirstChildElement("nd");
			auto roadType = m_Tag->FindAttribute("v")->Value();

			if (IsRoad(roadType))
			{
				Way* way_temp = new Way();

				way_temp->m_Id = m_Way->FindAttribute("id")->Int64Value();

				while (m_Tag != nullptr && strcmp(m_Tag->FindAttribute("k")->Value(), "oneway"))
				{
					m_Tag = m_Tag->NextSiblingElement("m_Tag");
				}

				if (m_Tag != nullptr)
				{
					if (strcmp(m_Tag->FindAttribute("v")->Value(), "yes"))
					{
						way_temp->m_OneWay = false;
					}
					else
					{
						way_temp->m_OneWay = true;
					}
				}
				else
				{
					way_temp->m_OneWay = false;
				}

				m_Nd = m_Way->FirstChildElement("nd");
				while (m_Nd != nullptr)
				{
					auto ref = m_Nd->FindAttribute("ref")->Int64Value();

					m_Route_Node_Ids->push_back(m_Nodes->at(ref));
					m_Nd = m_Nd->NextSiblingElement("nd");
				}

				way_temp->m_IdFrom = m_Route_Node_Ids->at(0)->m_Id;
				way_temp->m_IdTo = m_Route_Node_Ids->at(m_Route_Node_Ids->size() - 1)->m_Id;

				way_temp->m_Length = CalculateLength(m_Route_Node_Ids);

				m_Ways->push_back(way_temp);
			}
		}

		m_Way = m_Way->NextSiblingElement("way");
	}
}

void Converter::SaveToJson(const char* fileName)
{
	Json::Value nodes(Json::arrayValue);

	for (auto it = m_Nodes->cbegin(); it != m_Nodes->cend(); it++)
	{
		Json::Value node;
		node["node"]["id"] = it->second->m_Id;
		node["node"]["lat"] = it->second->m_Lat;
		node["node"]["lon"] = it->second->m_Lon;

		nodes.append(node);
	}

	Json::Value ways(Json::arrayValue);

	for (auto it = m_Ways->begin(); it != m_Ways->cend(); it++)
	{
		Json::Value way;
		way["way"]["id"] = (*it)->m_Id;
		way["way"]["idfrom"] = (*it)->m_IdFrom;
		way["way"]["idto"] = (*it)->m_IdTo;
		way["way"]["length"] = (*it)->m_Length;
		way["way"]["oneway"] = (*it)->m_OneWay;

		ways.append(way);
	}

	Json::Value doc;
	doc["nodes"] = nodes;
	doc["ways"] = ways;

	Json::StreamWriterBuilder builder;

	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream outputFileStream;
	std::string je = "highwaydata.json";
	outputFileStream.open(je);
	writer->write(doc, &outputFileStream);
	outputFileStream.close();
}

void Converter::ConvertOsmDataToJson(const char* osmFileName, const char* jsonFileName)
{
	LoadOsmFile(osmFileName);
	SelectNodesNeeded();
	LoadHighwayNodes();
	LoadHighways();
	SaveToJson(jsonFileName);
}

void Converter::ReadPreprocessedDataFromJson(const char* fileName)
{
	std::unordered_map<int64_t, Junction*>* Junctions = new std::unordered_map<int64_t, Junction*>();
	std::vector<Segment*>* Segments = new std::vector<Segment*>();
	Json::Value root;
	LoadJsonFile(fileName, root);
	GetPreprocessedData(root, Junctions, Segments);

}

float_t Converter::CalculateDistanceBetweenTwoLatLonsInMetres(const float_t lat1, const float_t lat2, const float_t lon1, const float_t lon2)
{

	const float_t radLat1 = lat1 * std::numbers::pi / 180;
	const float_t radLat2 = lat2 * std::numbers::pi / 180;

	const float_t dLat = (lat2 - lat1) * std::numbers::pi / 180;
	const float_t dLon = (lon2 - lon1) * std::numbers::pi / 180;

	const float_t a = std::sinf(dLat / 2) * std::sinf(dLat / 2) +
					std::cosf(radLat1) * std::cosf(radLat2) *
					std::sinf(dLon / 2) * std::sinf(dLon / 2);

	const float_t c = 2 * std::atan2f(std::sqrtf(a), std::sqrtf(1 - a));

	const float_t d = earthRadius * c;

	return d;
}

float_t Converter::CalculateLength(const std::vector<Node*>* nodeIds)
{
	float_t length = 0;
	for (size_t i = 1; i < nodeIds->size(); i++)
	{
		length += CalculateDistanceBetweenTwoLatLonsInMetres(
				nodeIds->at(i - 1)->m_Lat, nodeIds->at(i)->m_Lat,
				nodeIds->at(i - 1)->m_Lon, nodeIds->at(i)->m_Lon);
	}

	return length;
}

bool Converter::IsRoad(const char* roadType)
{
	if (strcmp(roadType, "motorway") &&
		strcmp(roadType, "trunk") && 
		strcmp(roadType, "primary") &&
		strcmp(roadType, "secondary") &&
		strcmp(roadType, "tertiary") &&
		strcmp(roadType, "unclassified") &&
		strcmp(roadType, "residential") &&
		strcmp(roadType, "unclassified") &&
		strcmp(roadType, "motorway_link") &&
		strcmp(roadType, "trunk_link") &&
		strcmp(roadType, "primary_link") &&
		strcmp(roadType, "secondary_link") &&
		strcmp(roadType, "tertiary_link"))
	{
		return false;
	}
	else
	{
		return true;
	}
}
