#include "SceneFactrory.h"
#include "EditorScene.h"

SceneFactrory* SceneFactrory::GetInstance()
{
	static SceneFactrory lInstance;
	return &lInstance;
}

std::unique_ptr<BaseScene> SceneFactrory::CreateScene(const std::string& sceneName)
{
	std::unique_ptr<BaseScene> lNewScene = nullptr;
	if ( sceneName == "EDITOR" )
	{
		lNewScene = std::make_unique<EditorScene>();
	}

	return std::move(lNewScene);
}
