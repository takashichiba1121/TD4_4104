#pragma once
#include"Vector2.h"
#include"PlayerAttack.h"
#include<memory>
#include<string>
class Player
{
private:
#pragma region ステータス

	//移動

	float topSpeed_ = 0;

	float acceleration_ = 0;

	float airAcceleration_ = 0;

	float deccelaration_ = 1;

	float airDeccelaration_ = 0;

	//戦闘

	//ジャンプ

	float gravityAcceleration_ = 0;

	float jumpAcceleration_ = 0;

	float jumpInitialVelocity_ = 0;

#pragma endregion

#pragma region 変更ステータス
	float changeSpd_=1;

	float changePow_=1;

	float changeDef_=1;

	float changeMaxHp_=1;

	int32_t nowCost=0;
#pragma endregion

	bool direction_ = false;

	float speed_ = 0;

	Vector2 pos_;

	Vector2 drawSize_ = { 34,60 };

	Vector2 colisionSize_ = { 29,55 };

	Vector2 colisionSift_ = { 5,5 };

	bool isAttack_ = false;

	uint32_t attackInterval_ = 0;

	std::unique_ptr<PlayerAttack> attackZ_;

	std::unique_ptr<PlayerAttack> attackX_;

	int32_t maxHp_ = 100;

	int32_t hp_ = maxHp_;

	bool onGround_ = false;

	bool isJump_ = false;

	float fallSpeed_ = 0;

public:
	void Initialze();

	void Update();

	void Move();

	void JumpStart();

	void Jump();

	void Falling();

	void Attack();

	float IsDamage();

	void OnCollsionEnemy(int32_t Damage);

	void ChangeAttackZ(std::string attackName);

	void ChangeAttackX(std::string attackName);

	void AddSpd(int32_t spd);

	void AddPow(int32_t pow);

	void AddDef(int32_t def);

	void AddMaxHp(int32_t maxHp);

	void AddCost(int32_t cost);

	void Draw();

private:

	void Load();

};
