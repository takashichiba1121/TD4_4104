#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"

class SceneManager
{
private:

	std::unique_ptr<BaseScene> scene_;
	std::unique_ptr<BaseScene> nextScene_;
	AbstractSceneFactory* sceneFactory_ = nullptr;
	int32_t screen_;
public:

	static SceneManager* GetInstance();

	void ChangeScene(const std::string& sceneName_);
	void SetScreenGraph(int32_t screen);

	void Update();
	void UIUpdate();

	void Draw();
	void UIDraw();
	void ProcessingDraw();

	void SetSceneFactory(AbstractSceneFactory* factory_);

	void Finalize();


private:

	~SceneManager() = default;
	SceneManager() = default;

	//コピーコンストラクタ・代入演算子削除
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
};
