#pragma once
#include<string>
#include<memory>

#include"BaseScene.h"

class AbstractSceneFactory
{
public:

	virtual ~AbstractSceneFactory() = default;

	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName_) = 0;
};
