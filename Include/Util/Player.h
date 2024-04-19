#pragma once
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

#pragma region ジャンプ用変数
	bool onGround_ = false;

	float fallSpeed_ = 0;

	float gravityAcceleration_ = 0.1f;

	float StartJumpSpeed_ = -7;
#pragma endregion


};

