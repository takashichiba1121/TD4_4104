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
#include"DealDaemon.h"

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
	Cerberus,
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

struct NowPartsTag
{
	PlayerAttackTags leftAtaackTag = PlayerAttackTags::Fist;

	PlayerAttackTags rightAtaackTag = PlayerAttackTags::Fist;

	PlayerLegTags legTag = PlayerLegTags::Normal;

	PlayerMouthTags mouthTag = PlayerMouthTags::Normal;

	PlayerEyeTags eyeTag = PlayerEyeTags::Normal;

	std::string leftAtaackName = "Fist";

	std::string rightAtaackName = "Fist";

	std::string legName = "Normal";

	std::string mouthName = "Normal";

	std::string eyeName = "Normal";
};

class Player:public BaseObject
{
private:

#pragma region ステータス変更値
	float changeMaxHp_=1;

	float changeSpd_=1;

	float changePow_=1;

	float changeDef_=1;

	float changeCrit_ = 0.1f;

	float changeCdmg_ = 1.5f;

	int32_t nowCost_=0;
#pragma endregion

#pragma region ステータス初期値

	const uint32_t MAX_HP_ = 150;

	uint32_t DEF_ = 0;


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

	bool isPowerUp_ = false;

	bool isChangeParts_ = false;

	uint32_t powerUpNum_ = 0;

	bool isDealed_ = false;

	bool powerUpText_=false;

	bool changePartsText_ = false;

	NowPartsTag nowPartTag_;

	uint32_t nowEyeCost_=0;

	uint32_t nowMouthCost_ = 0;

	Vector2 scroll_ = {0,0};
public:
	void Initialize() override;

	void Update() override;

	void Attack();

	void Damage(int32_t damage) override;

	void IventDamage(int32_t damage);

	bool ChangeLeftArm(std::string attackName,uint32_t cost);

	bool ChangeRightArm(std::string attackName,uint32_t cost);

	bool ChangeLeg(std::string legName,uint32_t cost);

	bool ChangeEye(std::string eyeName,uint32_t cost);

	bool ChangeMouth(std::string mouthName,uint32_t cost);

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

	void Draw(Vector2 scroll) override;

	bool ItemGet(Item newItem);

	void UseItem();

	uint32_t PowerUp();

	void EndPowerUp();

	void EndChangeParts();

	void Reset();

	bool IsPowerUp() {
		return isPowerUp_;
	}

	bool IsChangeParts() {
		return isChangeParts_;
	}

	void OnCollision() override;

	CircleShape* GetCircleShape()
	{
		return circelShape_.get() ;
	}

	PlayerAttackTags GetLeftAtaackTag()
	{
		return nowPartTag_.leftAtaackTag;
	}

	PlayerAttackTags GetRightAtaackTag()
	{
		return nowPartTag_.rightAtaackTag;
	}

	PlayerLegTags GetLegTag()
	{
		return nowPartTag_.legTag;
	}

	PlayerMouthTags GetMouthTag()
	{
		return nowPartTag_.mouthTag;
	}

	PlayerEyeTags GetEyeTag()
	{
		return nowPartTag_.eyeTag;
	}

	bool CheckHavePart(PartsName partType,std::string partName);

	void SoulMouth();

	uint32_t GetMaxHp()
	{
		return MAX_HP_ * changeMaxHp_;
	}

	void SetScroll(Vector2 scroll)
	{

	}
};
