#pragma once
#include"Core.h"
#include<string>
#include<array>
class GameConfig
{
public:

	struct Config
	{
		std::string title;
		int32_t windowWidth;
		int32_t windowHeight;
		int8_t backGroundColorR;
		int8_t backGroundColorB;
		int8_t backGroundColorG;
		int8_t fps;
		bool vSync;
		bool fullScreen;
	};

	struct  Node
	{
		int32_t xDistance;
		int32_t yDistance;
		int32_t placementRandomness;
		int32_t floors;
		int32_t width;
		int32_t paths;
		int32_t startPoints;
		std::array<int32_t,5> nodeProbabilities;
	};

	struct Boss
	{
		struct Attack
		{
			int32_t time;
			int32_t sizeX;
			int32_t sizeY;
			int32_t power;
		};

		struct Charge
		{
			int32_t time;
			int32_t speed;
			int32_t sizeX;
			int32_t sizeY;
			int32_t power;
		};

		int32_t hp;
		float attackInterval;
		int32_t approachHitBoxX;
		int32_t approachHitBoxY;
		Attack attack;
		Charge charge;
	};

private:
	Config config_;
	Node node_;
	Boss boss_;
public:
	static void Load(const std::string& filePath);

	static int32_t GetWindowWidth();
	static int32_t GetWindowHeight();
	static GameConfig::Config* GetGameConfig();
	static GameConfig::Node* GetNodeConfig();
	static GameConfig::Boss* GetBossConfig();

private:

	static GameConfig* GetInstance();

	GameConfig() = default;
	~GameConfig() = default;

};

