#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include<memory>
#include"IObject.h"
#include"RectShape.h"
#include"PlayerAttack.h"
class PlayerBullet:public IObject
{
public:
	enum class Type
	{
		Normal,
		ICED,
		BURN,
	};


protected:
	Vector2 position_;
	//速度
	Vector2 velocity_;
	//デスタイマー
	uint32_t life_;
	//デスフラグ
	bool isDead_ = false;

	float playerPow_;

	const Vector2 SIZE_ = { 10,10 };

	RectShape* shape_;

	float pow_;

	float  playerCrit_;

	float  playerCdmg_;

	Type type_;

public:

	///<summary>
	///初期化処理
	///</summary>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	///<param name="life">消えるまでの時間</param>
	void Initialize(Vector2 velocity,Vector2 position,uint32_t life,float PlayerPow,float changeCrit_,float changeCdmg_,Type type);

	///<summary>
	///毎フレーム処理
	///</summary>
	void Update();

	///<summary>
	///描画
	///</summary>
	void Draw();

	///<summary>
	///衝突を検出したら呼び出されたるコールバック関数
	///</summary>
	void OnCollision() override;

/// <summary>
/// 消えるべきか
/// </summary>
	bool IsDead() {
		return isDead_;
	}

};

