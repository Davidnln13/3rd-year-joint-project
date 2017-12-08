#include "LevelLoader.h"
#include <fstream>
#include <iostream>

LevelLoader::LevelLoader()
{
	std::ifstream ifs ("./resources/Data.json", std::ifstream::in);

	json j;

	ifs >> j;

	ifs.close();

	auto position = j["Lights"][0]["PosY"].at(1);
	auto name = j["Game Developers"].at(0);

	std::cout << name;
	std::cout << position;
}
