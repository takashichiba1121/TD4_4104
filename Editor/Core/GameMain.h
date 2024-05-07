#pragma once
#include"Framework.h"

class GameMain final : public FrameWork
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;
	void UIUpdate()override;

	/// <summary>
	/// 描画
	/// </summary>
	void EditorDraw() override;
	void UIDraw() override;

	bool UserEndRequst()override;

	GameMain() = default;
	~GameMain() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	GameMain& operator=(const GameMain&) = delete;
	GameMain(const GameMain&) = delete;
};

