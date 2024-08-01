#pragma once
#include"Vector2.h"
#include"PlayerAttack.h"
#include"CircleShape.h"
#include<memory>
#include<string>
#include"BaseObject.h"
#include"PlayerLeg.h"
#include"ItemShop.h"
#include"PowerUpCave.h"

struct UserData
{
	std::string tag;
};

enum class PlayerAttackTags
{
	Fist,
	Cerberus,
	Fenrir,
	Gun,
	Mars,
	Spider,
	Vine,
};
enum class PlayerLegTags
{
	Normal,
	Fenrir,
};

enum class PlayerMouthTags
{
	Normal,
	Soul,
};

enum class PlayerEyeTags
{
	Normal,
	Clairvoyance,
	Curse,
};

class Player:public BaseObject
{
private:

#pragma region ステータス変更値
	float changeMaxHp_=1;

	float changeSpd_=1;

	float changePow_=1;

	float changeDef_=1;

	float changeCrit_ = 0.1;

	float changeCdmg_ = 1.5;

	int32_t nowCost_=0;
#pragma endregion

#pragma region ステータス実数値

	const uint32_t MAX_HP_ = 150;

	const uint32_t DEF_ = 80;


#pragma endregion

	bool direction_ = false;

	Vector2 colisionSift_ = { 5,5 };

	RectShape* shape_;

	std::unique_ptr<CircleShape> circelShape_;

	std::unique_ptr<PlayerAttack> leftArm_;

	std::unique_ptr<PlayerAttack> rightArm_;

	std::unique_ptr<PlayerLeg> leg_;

	UserData name_;

	const uint32_t DAMAGE_INTERVAL_MAX_ = 16;

	uint32_t DamageInterval_ = DAMAGE_INTERVAL_MAX_;

	std::list<Item> items_;

	uint32_t selectItems_ = 1;

	std::unique_ptr<PowerUpCave>powerUp_;

	bool isPowerUp_ = false;

	uint32_t powerUpNum_ = 0;

	bool isDealed_ = false;

	bool powerUpText_=false;

	PlayerAttackTags leftAtaackTag_=PlayerAttackTags::Fist;

	PlayerAttackTags rightAtaackTag_ = PlayerAttackTags::Fist;

	PlayerLegTags legTag_ = PlayerLegTags::Normal;

	PlayerMouthTags mouthTag_ = PlayerMouthTags::Normal;

	PlayerEyeTags eyeTag_ = PlayerEyeTags::Normal;


public:
	void Initialize() override;

	void Update() override;

	void Attack();

	void Damage(int32_t Damage) override;

	bool ChangeLeftArm(std::string attackName,uint32_t cost);

	bool ChangeRightArm(std::string attackName,uint32_t cost);

	bool ChangeLeg(std::string legName,uint32_t cost);

	bool AddSpd(int32_t spd);

	bool AddPow(int32_t pow);

	bool AddDef(int32_t def);

	bool AddMaxHp(int32_t maxHp);

	bool AddCrit(float Crit);

	bool AddCdmg(int32_t Cdmg);

	bool AddCost(int32_t cost);

	bool SubSpd(int32_t spd);

	bool SubPow(int32_t pow);

	bool SubDef(int32_t def);

	bool SubMaxHp(int32_t maxHp);

	bool SubCrit(int32_t Crit);

	bool SubCdmg(int32_t Cdmg);

	int32_t GetCost() {
		return nowCost_;
	}

	void Draw() override;

	bool ItemGet(Item newItem);

	void UseItem();

	uint32_t PowerUp();

	void EndPowerUp();

	void Reset();

	bool IsPowerUp() {
		return isPowerUp_;
	}

	void OnCollision() override;

	CircleShape* GetCircleShape()
	{
		return circelShape_.get() ;
	}

	PlayerAttackTags GetLeftAtaackTag()
	{
		return leftAtaackTag_;
	}

	PlayerAttackTags GetRightAtaackTag()
	{
		return rightAtaackTag_;
	}

	PlayerLegTags GetLegTag()
	{
		return legTag_;
	}

	PlayerMouthTags GetMouthTag()
	{
		return mouthTag_;
	}

	PlayerEyeTags GetEyeTag()
	{
		return eyeTag_;
	}
};
