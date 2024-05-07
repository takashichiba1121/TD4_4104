#pragma once

#include<memory>

#include"DxlibManager.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
class FrameWork
{
private:

	SceneManager* sceneManager_;

	std::unique_ptr<DxlibManager>dxlibManager_;
	std::unique_ptr<ImGuiManager>imGuiManager_;

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
	virtual void EditorDraw();

	virtual bool UserEndRequst();

	virtual void UIDraw();

	virtual void UIUpdate();

	bool EndRequst();

	/// <summary>
	/// 実行
	/// </summary>
	void Run();
};

