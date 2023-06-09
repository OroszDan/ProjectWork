// ProjectWork.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RoutePlanner.h"

//int JsonCreate() 
//{
//    Json::Value event;
//    Json::Value vec(Json::arrayValue);
//    vec.append(Json::Value(1));
//    vec.append(Json::Value(2));
//    vec.append(Json::Value(3));
//
//    event["competitors"]["home"]["name"] = "Liverpool";
//    event["competitors"]["home"]["code"] = 89223;
//    event["competitors"]["away"]["name"] = "Aston Villa";
//    event["competitors"]["away"]["code"] = vec;
//
//    Json::StreamWriterBuilder builder;
//
//    builder["commentStyle"] = "None";
//    builder["indentation"] = "   ";
//
//    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
//    std::ofstream outputFileStream;
//    outputFileStream.open("test.json");
//    writer->write(event, &outputFileStream);
//    outputFileStream.close();
//
//   return 0;
//}

int main()
{
    /*std::shared_ptr<std::vector<Junction>> resultJunctions = std::make_shared<std::vector<Junction>>();

    Junction junction = Junction(2342, 3232, 232);

    resultJunctions->push_back(junction);

    auto je = &resultJunctions->back();

    std::cout << je << std::endl;

    auto jeboi = &resultJunctions->back();

    std::cout << jeboi << std::endl;*/


    //std::unique_ptr<Converter> converter = std::make_unique<Converter>();

    ////converter->ConvertOsmDataToJson("liechtenstein-latest.osm", "highwaydata.json");
    //converter->ConvertOsmDataToJson("luxembourg-latest.osm", "highwaydata.json");





    std::unique_ptr<RoutePlanner> planner = std::make_unique<RoutePlanner>();

    std::shared_ptr<std::vector<const Junction*>> resultJunctions = std::make_shared<std::vector<const Junction*>>();

    planner->Initialize();
    //planner->Search(346146156, 32020993);
    //planner->Search(346146156, 30604007, resultJunctions);
    planner->Search((float)47.243446350097656f, (float)9.5248165130615234f, (float)47.244438171386719f, (float)9.5273571014404297f, resultJunctions);

    Converter::SaveResultToGeoJson(resultJunctions, "result.geojson");
    ;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
