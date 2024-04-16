#pragma once

#include<memory>

#include"GameConfig.h"
#include"DxlibManager.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"FPS.h"

class FrameWork
{
private:

	SceneManager* sceneManager_ = nullptr;

	std::unique_ptr<ImGuiManager>imguiManager_;
	std::unique_ptr<DxlibManager>dxlibManager_;
	std::unique_ptr<FPS>fps_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void PostUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	virtual bool UserEndRequst();


	bool EndRequst();

	/// <summary>
	/// 実行
	/// </summary>
	void Run();
};

