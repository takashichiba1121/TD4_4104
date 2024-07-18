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

	{
		nlohmann::json& object = jsonObject[ "Config" ];

		assert(object.is_object());
		assert(object.contains("name"));
		assert(object[ "name" ].is_string());

		std::string lName = object[ "name" ].get<std::string>();

		assert(lName.compare("Config") == 0);

		Config& config = GetInstance()->config_;

		config.title = object[ "Title" ];
		config.windowWidth = object[ "WindowSize" ][ 0 ];
		config.windowHeight = object[ "WindowSize" ][ 1 ];
		config.vSync = object[ "VSync" ];
		config.fps = object[ "FPS" ];
		config.fullScreen = object[ "FullScreen" ];
		std::string color = object[ "BackGroundColor" ][ 0 ];
		config.backGroundColorR = strtol(color.c_str(),nullptr,0);
		object[ "BackGroundColor" ][ 1 ];
		config.backGroundColorG = strtol(color.c_str(),nullptr,0);
		object[ "BackGroundColor" ][ 1 ];
		config.backGroundColorB = strtol(color.c_str(),nullptr,0);
	}
	{
		nlohmann::json& object = jsonObject[ "Node" ];
		assert(object.is_object());
		assert(object.contains("name"));
		assert(object[ "name" ].is_string());

		std::string lName = object[ "name" ].get<std::string>();

		assert(lName.compare("Node") == 0);

		Node& node = GetInstance()->node_;

		node.xDistance = object[ "XDistance" ];
		node.yDistance = object[ "YDistance" ];
		node.placementRandomness = object[ "PlacementRandomness" ];
		node.floors = object[ "Floors" ];
		node.width = object[ "Width" ];
		node.paths = object[ "Paths" ];
		node.startPoints = object[ "StartPoints" ];
		for ( size_t i = 0; i < node.nodeProbabilities.size(); i++ )
		{
			node.nodeProbabilities[i] = object[ "NodeProbabilities" ][i];
		}
	}
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

GameConfig::Node* GameConfig::GetNodeConfig()
{
	return &GetInstance()->node_;
}

GameConfig* GameConfig::GetInstance()
{
	static GameConfig instance;
	return &instance;
}