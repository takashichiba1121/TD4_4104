#pragma once
#include"PlayerAttack.h"
#include<memory>
class Player
{
public:
	void Initialze();

	void Update();

	void Jump();

	void Attack();

	void Draw();
private:
	const float cSpeed_ = 5.0f;

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

