#include "RoutePlanner.h"

RoutePlanner::RoutePlanner()
{
    m_Junctions = std::make_shared<std::unordered_map<int64_t, Junction*>>();
    m_Segments = std::make_shared<std::vector<Segment*>>();
}

void RoutePlanner::Initialize()
{
    Converter* converter = new Converter();

    converter->ReadPreprocessedDataFromJson("highwaydata.json", m_Junctions, m_Segments);
    //converter->ConvertOsmDataToJson("liechtenstein-latest.osm", "highwaydata.json");

    converter->~Converter();
}

void RoutePlanner::Search(const int64_t idFrom, const int64_t idTo) 
{
    //Dijkstra algorithm

    std::unordered_map<int64_t, int64_t>* S = new std::unordered_map<int64_t, int64_t>();
    bool routeFound = false;

    for (auto it = m_Junctions->cbegin(); it != m_Junctions->cend(); ++it) 
    {
        S->insert(std::make_pair(it->first, it->first));
    }

    std::vector<Junction*>* LE = new std::vector<Junction*>();
    //start
    Junction* start = m_Junctions->at(idFrom);
    start->m_ShortestRouteInMetres = 0;
    LE->push_back(start);

    while (S->size() > 0 && !routeFound)
    {
        //minkivesz
        Junction* u = GetMin(S, LE);
        for(auto it = u->m_Segments->begin(); it != u->m_Segments->end(); ++it)
        {
            Junction* endJunction = (*it)->GetEndJunction(u);
            //insert neighbor 
            LE->push_back(endJunction);

            if (u->m_ShortestRouteInMetres + (*it)->m_Length < endJunction->m_ShortestRouteInMetres)
            {
                endJunction->m_ShortestRouteInMetres = u->m_ShortestRouteInMetres + (*it)->m_Length;
                endJunction->m_ShortestRouteNeighbor = *it;
            }

            if (endJunction->m_Id == idTo)
            {
                routeFound = true;
            }
        }
    }

    ;

}

Junction* RoutePlanner::GetMin(std::unordered_map<int64_t, int64_t>* s, std::vector<Junction*>* LE)
{// szar
    //megoldás lehet esetleg
    //converter is szar mert vannak benne node-ok amelyek nem kellenek
    auto minIt = 
    std::min_element(LE->begin(), LE->end(), [s](const Junction* elem1, const Junction* elem2)
    {
        if (s->contains(elem1->m_Id))
        {
            return elem1->m_ShortestRouteInMetres < elem2->m_ShortestRouteInMetres;
        }
        else
        {
            return false;
        }
        
    });
    s->erase((*minIt)->m_Id);

    return *minIt;
}
