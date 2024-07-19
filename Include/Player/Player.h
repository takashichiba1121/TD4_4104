#pragma once
#include"Vector2.h"
#include"PlayerAttack.h"
#include"CircleShape.h"
#include<memory>
#include<string>
#include"BaseObject.h"
#include"PlayerLeg.h"
#include"PlayerLegNormal.h"
#include"ItemShop.h"

struct UserData
{
	std::string tag;
};

class Player:public BaseObject
{
private:

#pragma region 変更ステータス
	float changeSpd_=1;

	float changePow_=1;

	float changeDef_=1;

	float changeCrit_ = 0.1;

	float changeCdmg_ = 1.5;

	int32_t nowCost=0;
#pragma endregion

	bool direction_ = false;

	Vector2 colisionSift_ = { 5,5 };

	RectShape* shape_;

	std::unique_ptr<PlayerAttack> leftArm_;

	std::unique_ptr<PlayerAttack> rightArm_;

	std::unique_ptr<PlayerLeg> leg_;

	int32_t maxHp_ = 100;

	UserData name_;

	const uint32_t DAMAGE_INTERVAL_MAX_ = 16;

	uint32_t DamageInterval_ = DAMAGE_INTERVAL_MAX_;

	std::list<Item> items_;

	uint32_t selectItems_ = 1;
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
		return nowCost;
	}

	void Draw() override;

	bool ItemGet(Item newItem);

	void UseItem();
};
