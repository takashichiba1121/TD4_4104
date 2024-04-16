#include "SceneManager.h"
#include<cassert>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::Update()
{
	//次のシーンの予約があるなら
	if ( nextScene_ )
	{
		//旧シーンの終了
		if ( scene_ )
		{
			scene_->Finalize();
			scene_.reset();
		}

		//シーン切り替え
		scene_ = std::move(nextScene_);

		//シーンマネージャーをセット
		scene_->SetSceneManager(this);

		//次のシーンを初期化する
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* factory)
{
	sceneFactory_ = factory;
}

void SceneManager::Finalize()
{
	if ( scene_ )
	{
		scene_->Finalize();
		scene_.reset();
	}
}