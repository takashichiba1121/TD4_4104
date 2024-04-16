#pragma once
#include"Core.h"
#include<string>

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

private:
	Config config_;
public:
	static void Load(const std::string& filePath);

	static int32_t GetWindowWidth();
	static int32_t GetWindowHeight();
	static GameConfig::Config* GetGameConfig();

private:

	static GameConfig* GetInstance();

	GameConfig() = default;
	~GameConfig() = default;

};

