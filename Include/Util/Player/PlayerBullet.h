#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include<memory>
class PlayerBullet
{
public:
	enum class Phase
	{
		Charge,
		Attack,
		Delete,
	};
public:

	///<summary>
	///初期化処理
	///</summary>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	///<param name="life">消えるまでの時間</param>
	void Initialize(Vector2 velocity,Vector2 position,uint32_t life);

	///<summary>
	///毎フレーム処理
	///</summary>
	void Update();

	void Attack();

	///<summary>
	///描画
	///</summary>
	void Draw();

	///<summary>
	///衝突を検出したら呼び出されたるコールバック関数
	///</summary>
	virtual void OnCollision();

/// <summary>
/// 消えるべきか
/// </summary>
	bool IsDead() {
		return isDead_;
	}

	void SetPhase(Phase phase);

	void SetChageTime(uint32_t Time) {
		chageTime_ = Time;
	}

protected:
	Vector2 position_;
	//速度
	Vector2 velocity_;
	//デスタイマー
	uint32_t life_;
	//デスフラグ
	bool isDead_ = false;

		//弾のチャージ時間
	uint32_t chageTime_ = 60;

	uint32_t chageTimer_ = 0;

	Phase phase_ = Phase::Charge;

};

