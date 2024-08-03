#include "SceneFactrory.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"

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
	else if ( sceneName == "TITLE" )
	{
		lNewScene = std::make_unique<TitleScene>();
	}
	else if ( sceneName == "CLEAR" )
	{
		lNewScene = std::make_unique<ClearScene>();
	}
	else if( sceneName == "GAMEOVER" )
	{
		lNewScene = std::make_unique<GameOverScene>();
	}

	return std::move(lNewScene);
}
