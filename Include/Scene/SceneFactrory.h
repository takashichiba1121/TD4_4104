#pragma once
#include"AbstractSceneFactory.h"

class SceneFactrory :public AbstractSceneFactory
{
public:

	static SceneFactrory* GetInstance();

	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;
};

