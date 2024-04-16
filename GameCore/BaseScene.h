#pragma once
#include"Core.h"

class SceneManager;

class BaseScene
{
protected:

	SceneManager* sceneManager_ = nullptr;

public:

	virtual ~BaseScene() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void SpriteDraw() = 0;

	virtual void Finalize() = 0;

	void SetSceneManager(SceneManager* manager);

};
