#pragma once
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

class LevelLoader
{
public:
	LevelLoader();

	//Getters
	json& data();

private:
	json m_loadedData;
	//std::vector<> floorVector;
};
