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
			node.nodeProbabilities[ i ] = object[ "NodeProbabilities" ][ i ];
		}
	}

	{
		nlohmann::json& object = jsonObject[ "Boss" ];
		assert(object.is_object());
		assert(object.contains("Name"));
		assert(object[ "Name" ].is_string());

		std::string lName = object[ "Name" ].get<std::string>();

		assert(lName.compare("Boss") == 0);

		Boss& boss = GetInstance()->boss_;

		boss.hp = object[ "Hp" ];
		boss.attackInterval = object[ "AttackInterval" ];
		boss.approachHitBoxX = object[ "ApproachHitBox" ][ 0 ];
		boss.approachHitBoxY = object[ "ApproachHitBox" ][ 1 ];

		{
			nlohmann::json& attackObject = object[ "Attack" ];
			boss.attack.sizeX = attackObject[ "Size" ][ 0 ];
			boss.attack.sizeY = attackObject[ "Size" ][ 1 ];
			boss.attack.time = attackObject[ "Time" ];
			boss.attack.power = attackObject[ "Power" ];
		}

		{
			nlohmann::json& chargeObject = object[ "Charge" ];
			boss.charge.sizeX = chargeObject[ "Size" ][ 0 ];
			boss.charge.sizeY = chargeObject[ "Size" ][ 1 ];
			boss.charge.time = chargeObject[ "Time" ];
			boss.charge.power = chargeObject[ "Power" ];
			boss.charge.speed = chargeObject[ "Speed" ];
		}

		{
			nlohmann::json& longRangeObject = object[ "LongRange" ];
			boss.longRange.chargeTime = longRangeObject[ "ChargeTime" ];
			boss.longRange.freezeTime = longRangeObject[ "FreezeTime" ];

			{
				nlohmann::json& bulletObject = longRangeObject[ "Bullet" ];
				boss.longRange.bulletTime = bulletObject[ "Time" ];
				boss.longRange.bulletSpeed = bulletObject[ "Speed" ];
				boss.longRange.bulletSizeX = bulletObject[ "Size" ][ 0 ];
				boss.longRange.bulletSizeY = bulletObject[ "Size" ][ 1 ];
				boss.longRange.bulletPower = bulletObject[ "Power" ];

			}
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

GameConfig::Boss* GameConfig::GetBossConfig()
{
	return &GetInstance()->boss_;
}

GameConfig* GameConfig::GetInstance()
{
	static GameConfig instance;
	return &instance;
}