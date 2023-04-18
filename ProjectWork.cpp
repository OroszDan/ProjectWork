// ProjectWork.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Converter.h"
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

int JsonCreate() 
{
    Json::Value event;
    Json::Value vec(Json::arrayValue);
    vec.append(Json::Value(1));
    vec.append(Json::Value(2));
    vec.append(Json::Value(3));

    event["competitors"]["home"]["name"] = "Liverpool";
    event["competitors"]["home"]["code"] = 89223;
    event["competitors"]["away"]["name"] = "Aston Villa";
    event["competitors"]["away"]["code"] = vec;

    Json::StreamWriterBuilder builder;

    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream;
    outputFileStream.open("test.json");
    writer->write(event, &outputFileStream);
    outputFileStream.close();

   return 0;
}

int main()
{

    Converter* converter = new Converter();

    converter->ReadPreprocessedDataFromJson("highwaydata.json");
    //converter->ConvertOsmDataToJson("liechtenstein-latest.osm", "highwaydata.json");

    converter->~Converter();

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
