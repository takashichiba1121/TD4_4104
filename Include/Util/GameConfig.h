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
		int xDistance;
		int yDistance;
		int placementRandomness;
		int floors;
		int width;
		int paths;
		int startPoints;
		std::array<int32_t,5> nodeProbabilities;
	};

private:
	Config config_;
	Node node_;
public:
	static void Load(const std::string& filePath);

	static int32_t GetWindowWidth();
	static int32_t GetWindowHeight();
	static GameConfig::Config* GetGameConfig();
	static GameConfig::Node* GetNodeConfig();

private:

	static GameConfig* GetInstance();

	GameConfig() = default;
	~GameConfig() = default;

};

