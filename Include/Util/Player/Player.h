#pragma once
#include"Vector2.h"
#include"PlayerAttack.h"
#include<memory>
#include<string>
class Player
{
private:
#pragma region 移動ステータス

	float topSpeed_ = 0;

	float acceleration_ = 0;

	float airAcceleration_ = 0;

	float deccelaration_ = 1;

	float airDeccelaration_ = 0;

#pragma endregion

	bool direction_ = false;

	float speed_ = 0;

	Vector2 pos_;

	Vector2 drawSize_ = { 35,61 };

	Vector2 colisionSize_ = { 5,5 };

	Vector2 colisionSift_ = { 5,5 };

	bool isAttack_ = false;

	uint32_t attackInterval_ = 0;

	std::unique_ptr<PlayerAttack> attackZ_;

	std::unique_ptr<PlayerAttack> attackX_;
	int32_t hp_;

	int32_t maxHp_ = 10;

#pragma region ジャンプ用変数
	bool onGround_ = false;

	bool isJump_ = false;

	float fallSpeed_ = 0;

	float gravityAcceleration_ = 0;

	float jumpAcceleration_ = 0;

	float jumpInitialVelocity_ = 0;
#pragma endregion

public:
	void Initialze();

	void Update();

	void Move();

	void JumpStart();

	void Jump();

	void Falling();

	void Attack();

	void ChangeAttack(std::string AttackName);

	void Draw();

private:

	void Load();

};
