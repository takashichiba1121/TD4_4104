#include "GameConfig.h"

#include"json.hpp"

#include <fstream>

void GameConfig::Load(const std::string& filePath)
{
	std::ifstream file;

	file.open(filePath);

	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json jsonObject;

	file >> jsonObject;

	assert(jsonObject.is_object());
	assert(jsonObject.contains("name"));
	assert(jsonObject[ "name" ].is_string());

	std::string lName = jsonObject[ "name" ].get<std::string>();

	assert(lName.compare("Config") == 0);

	Config& config = GetInstance()->config_;

	config.title = jsonObject[ "Title" ];
	config.windowWidth = jsonObject[ "WindowSize" ][ 0 ];
	config.windowHeight = jsonObject[ "WindowSize" ][ 1 ];
	config.vSync = jsonObject[ "VSync" ];
	config.fps = jsonObject[ "FPS" ];
	config.fullScreen = jsonObject[ "FullScreen" ];
	std::string color = jsonObject[ "BackGroundColor" ][ 0 ];
	config.backGroundColorR = strtol(color.c_str(),nullptr,0);
	jsonObject[ "BackGroundColor" ][ 1 ];
	config.backGroundColorG = strtol(color.c_str(),nullptr,0);
	jsonObject[ "BackGroundColor" ][ 1 ];
	config.backGroundColorB = strtol(color.c_str(),nullptr,0);
}

int32_t GameConfig::GetWindowWidth()
{
	return GetInstance()->config_.windowWidth;
}

int32_t GameConfig::GetWindowHeight()
{
	return GetInstance()->config_.windowHeight;
}

GameConfig::Config* GameConfig::GetGameConfig()
{
	return &GetInstance()->config_;
}

GameConfig* GameConfig::GetInstance()
{
	static GameConfig instance;
	return &instance;
}