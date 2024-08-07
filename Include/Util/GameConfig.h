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
			int32_t offsetX;
			int32_t offsetY;
		};

		struct Charge
		{
			int32_t time;
			int32_t speed;
			int32_t sizeX;
			int32_t sizeY;
			int32_t power;
			int32_t animeFrame;
			int32_t anime2Frame;
		};

		struct LongRange
		{
			int32_t chargeTime;
			int32_t freezeTime;

			int32_t bulletTime;
			int32_t bulletSpeed;
			int32_t bulletSizeX;
			int32_t bulletSizeY;
			int32_t bulletPower;
			int32_t bulletRotateSpeed;
		};

		int32_t hp;
		float attackInterval;
		int32_t attackApproachHitBoxX;
		int32_t attackApproachHitBoxY;
		int32_t chargeApproachHitBoxY;
		int32_t chargeApproachHitBoxX;
		int32_t probabilitie1;
		int32_t probabilitie2;
		Attack attack;
		Charge charge;
		LongRange longRange;
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

