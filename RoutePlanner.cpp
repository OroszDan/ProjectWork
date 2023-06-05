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

    std::shared_ptr<std::unordered_map<int64_t, int64_t>> S = std::make_shared<std::unordered_map<int64_t, int64_t>>();
    bool routeFound = false;

    for (auto it = m_Junctions->cbegin(); it != m_Junctions->cend(); ++it) 
    {
        S->insert(std::make_pair(it->first, it->first));
    }

    std::shared_ptr<std::unordered_map<int64_t, Junction*>> LE = std::make_shared<std::unordered_map<int64_t, Junction*>>();
    //start
    Junction* start = m_Junctions->at(idFrom);
    start->m_ShortestRouteInMetres = 0;
   // LE->insert(start);
    LE->insert(std::make_pair(start->m_Id, start));

    while (S->size() > 0 && !routeFound)
    {
        //minkivesz
        Junction* u = GetMin(S, LE);

        for(auto it = u->m_Segments->begin(); it != u->m_Segments->end(); ++it)
        {
            Junction* endJunction = (*it)->GetEndJunction(u);
            //insert neighbor 

            if (u->m_ShortestRouteInMetres + (*it)->m_Length < endJunction->m_ShortestRouteInMetres)
            {
                endJunction->m_ShortestRouteInMetres = u->m_ShortestRouteInMetres + (*it)->m_Length;
                endJunction->m_ShortestRouteNeighbor = *it;   
            }

            if (S->contains(endJunction->m_Id) && !LE->contains(endJunction->m_Id))
            {
                LE->insert(std::make_pair(endJunction->m_Id,endJunction));
            }

            if (endJunction->m_Id == idTo)
            {
                routeFound = true;
            }
        }
    }

    ;

}

Junction* RoutePlanner::GetMin(std::shared_ptr<std::unordered_map<int64_t, int64_t>> S, std::shared_ptr<std::unordered_map<int64_t, Junction*>> LE)
{
    //megoldás lehet esetleg
    //converter is szar mert vannak benne node-ok amelyek nem kellenek
    auto minIt = 
    std::min_element(LE->begin(), LE->end(), [S](const std::pair<int64_t, Junction*> elem1, const std::pair<int64_t, Junction*> elem2)
    {
        return elem1.second->m_ShortestRouteInMetres < elem2.second->m_ShortestRouteInMetres; 
    });

    Junction* minElem = (*minIt).second;

    S->erase((*minIt).first);
    LE->erase(minIt);

    return minElem;
}
