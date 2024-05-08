#pragma once
#include"PlayerAttack.h"
#include<memory>
class Player
{
public:
	void Initialze();

	void Update();

	void Jump();

	void Draw();
private:
	const float cSpeed_ = 0.1f;

	const float cTopSpeed = 10.0f;

	const float cBrake = 0.5f;

	float accel = 0;

	float posX_;

	float posY_;

	bool isAttack_=false;

	std::unique_ptr<PlayerAttack> attack;

#pragma region ジャンプ用変数
	bool onGround_ = false;

	float fallSpeed_ = 0;

	float gravityAcceleration_ = 0.3f;

	float StartJumpSpeed_ = -14;
#pragma endregion


};

