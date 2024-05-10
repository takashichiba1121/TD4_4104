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
	const float cTopSpeed_ = 10.0f;

	const float cAcceleration_ = 0.1f;

	const float cAirAcceleration_ = 0.05f;

	const float cDeccelaration_ = 0.5f;

	const float cAirDeccelaration_ = 0.25f;

	float speed_ = 0;

	float posX_;

	float posY_;

	bool isAttack_=false;

	std::unique_ptr<PlayerAttack> attack_;

#pragma region ジャンプ用変数
	bool onGround_ = false;

	float fallSpeed_ = 0;

	float gravityAcceleration_ = 0.3f;

	float StartJumpSpeed_ = -14;
#pragma endregion

	
};

