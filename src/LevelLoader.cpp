#include "LevelLoader.h"

LevelLoader::LevelLoader()
{
	//Create an if stream
	std::ifstream ifs ("./resources/Data.json", std::ifstream::in);

	//Read the if stream into our variable
	ifs >> m_loadedData;

	//Close the stream
	ifs.close();

	//auto position = j["Lights"][0]["PosY"].at(1);
	//auto name = j["Game Developers"].at(0);
}

json& LevelLoader::data()
{
	return m_loadedData;
}
