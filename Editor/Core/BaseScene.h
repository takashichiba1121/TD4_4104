#pragma once
#include<stdint.h>

class SceneManager;

class BaseScene
{
protected:

	SceneManager* sceneManager_ = nullptr;

public:

	virtual ~BaseScene() = default;

	virtual void Initialize(int32_t screen_) = 0;

	virtual void Update() = 0;
	virtual void UIUpdate() = 0;

	virtual void Draw() = 0;
	virtual void UIDraw() = 0;
	virtual void ProcessingDraw() =0;

	virtual void SpriteDraw() = 0;

	virtual void Finalize() = 0;

	void SetSceneManager(SceneManager* manager);

};
