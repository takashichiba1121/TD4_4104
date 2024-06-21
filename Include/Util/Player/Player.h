#pragma once
#include"Vector2.h"
#include"PlayerAttack.h"
#include"CircleShape.h"
#include<memory>
#include<string>
#include"BaseObject.h"

struct UserData
{
	std::string tag;
};

class Player:public BaseObject
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
	float changeAcl_=1;

	float changePow_=1;

	float changeDef_=1;

	float changeMaxHp_=1;

	int32_t nowCost=0;
#pragma endregion

	bool direction_ = false;

	Vector2 colisionSift_ = { 5,5 };

	bool isAttack_ = false;

	uint32_t attackInterval_ = 0;

	RectShape* shape_;

	std::unique_ptr<PlayerAttack> attackZ_;

	std::unique_ptr<PlayerAttack> attackX_;

	int32_t maxHp_ = 100;

	bool onGround_ = false;

	bool isJump_ = false;

	float fallSpeed_ = 0;

	UserData name_;

	const uint32_t DAMAGE_INTERVAL_MAX_ = 16;

	uint32_t DamageInterval_ = DAMAGE_INTERVAL_MAX_;

	bool isEvasionRoll_ = false;
public:
	void Initialize() override;

	void Update() override;

	void Move();

	void JumpStart();

	void Jump();

	void EvasionRoll();

	void Falling();

	void Attack();

	float IsDamage();

	void Damage(int32_t Damage) override;

	void ChangeAttackZ(std::string attackName);

	void ChangeAttackX(std::string attackName);

	bool AddSpd(int32_t spd);

	bool AddPow(int32_t pow);

	bool AddDef(int32_t def);

	bool AddMaxHp(int32_t maxHp);

	bool AddCost(int32_t cost);

	int32_t GetCost() {
		return nowCost;
	}

	void Draw() override;



private:

	void Load();

};
