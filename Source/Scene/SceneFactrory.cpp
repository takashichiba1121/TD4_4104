#include "SceneFactrory.h"
#include "GameScene.h"

SceneFactrory* SceneFactrory::GetInstance()
{
	static SceneFactrory lInstance;
	return &lInstance;
}

std::unique_ptr<BaseScene> SceneFactrory::CreateScene(const std::string& sceneName)
{
	std::unique_ptr<BaseScene> lNewScene = nullptr;
	if ( sceneName == "GAME" )
	{
		lNewScene = std::make_unique<GameScene>();
	}

	return std::move(lNewScene);
}
