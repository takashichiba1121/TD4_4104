#pragma once
#include"PlayerAttack.h"
#include<memory>
class Player
{
public:
	void Initialze();

	void Update();

	void Move();

	void Jump();

	void Draw();
private:
	float cTopSpeed_ = 10.0f;

	float acceleration_ = 0.1f;

	float cAirAcceleration_ = 0.05f;

	float cDeccelaration_ = 1.0f;

	float cAirDeccelaration_ = 0.5f;

	float speed_ = 0;

	float posX_;

	float posY_;

	bool isAttack_=false;

	std::unique_ptr<PlayerAttack> attack_;

#pragma region ジャンプ用変数
	bool onGround_ = false;

	float fallSpeed_ = 0;

	float gravityAcceleration_ = 0.5f;

	float StartJumpSpeed_ = -18;
#pragma endregion


};

