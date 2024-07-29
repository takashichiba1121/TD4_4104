#include "NPCManager.h"

void NPCManager::Initialize()
{
	npcs_.clear();
	Load();
}

void NPCManager::Update()
{

	for ( auto& itr : npcs_ )
	{
		itr->Update();
	}

}

void NPCManager::Draw()
{

	for ( auto& itr : npcs_ )
	{

		itr->Draw();
	}

}

void NPCManager::SetLoadFile(std::string path)
{
	loadFilePath_ = path;
}

void NPCManager::Load()
{



}
