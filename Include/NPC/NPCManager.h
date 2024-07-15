#pragma once
#include "NPC.h"
#include <list>
class NPCManager
{
private:
	std::list<std::unique_ptr<NPC>> npcs_;
	std::string loadFilePath_;
	void Load();
public:
	void Initialize();
	void Update();
	void Draw();
	void SetLoadFile(std::string path);
};

