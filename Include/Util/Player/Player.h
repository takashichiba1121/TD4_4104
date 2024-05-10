#pragma once
#include"Vector2.h"
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
#pragma region 移動ステータス

	float topSpeed_ = 10.0f;

	float acceleration_ = 0.1f;

	float airAcceleration_ = 0.05f;

	float deccelaration_ = 1.0f;

	float airDeccelaration_ = 0.5f;

#pragma endregion

	bool direction_ = false;

	float speed_ = 0;

	Vector2 pos_;

	Vector2 size_ = { 35,61 };

	Vector2 colisionSize_ = { 5,5 };

	Vector2 colisionSift_ = { 5,5 };

	bool isAttack_ = false;

	std::unique_ptr<PlayerAttack> attack_;

#pragma region ジャンプ用変数
	bool onGround_ = false;

	float fallSpeed_ = 0;

	float gravityAcceleration_ = 0.5f;

	float jumpAcceleration_ = -18;
#pragma endregion


};
