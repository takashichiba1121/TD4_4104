#pragma once
#include"PlayerLeg.h"
class PlayerLegCerberus :
    public PlayerLeg
{
private:

	Vector2* playerVelocity_;

	bool* direction_;

	float* changeAcl_;

	bool onGround_;

	bool isJump_;

	bool isWalk;

	bool isEvasionRoll_;

	bool isBullet=false;

	uint32_t PlayerStandTexture_[ 10 ];

	uint32_t PlayerStandTextureCount_ = 0;

	uint32_t PlayerJumpTexture_[ 4 ];

	uint32_t PlayerJumpTextureCount_ = 0;

	uint32_t PlayerDownTexture_[ 4 ];

	uint32_t PlayerDownTextureCount_ = 0;

	uint32_t PlayerDushTexture_[ 5 ];

	uint32_t PlayerDushTextureCount_ = 0;

	const uint32_t DEF_=70;

#pragma region ステータス

//移動

	float topSpeed_ = 0;

	float acceleration_ = 0;

	float airAcceleration_ = 0;

	float deccelaration_ = 1;

	float airDeccelaration_ = 0;

	float evasionRollSpeed_ = 24;

	//戦闘

	//ジャンプ

	float gravityAcceleration_ = 0;

	float jumpAcceleration_ = 0;

	float jumpInitialVelocity_ = 0;

	bool isDirBottom_;

	uint32_t bulletInterval_;

	const uint32_t MAX_BULLET_INTERVAL_=5;

#pragma endregion

public:

	void Initialize(Vector2* playerPos,bool* direction,float* changeAcl) override;

	void Move(bool DirBOTTOM,bool isAttack,const Vector2& pos,const float pow) override;

	void JumpStart() override;

	void Jump() override;

	void EvasionRoll() override;

	void Falling() override;

	void Draw(const Vector2& pos,const Vector2& size) override;

	void Load() override;

	uint32_t GetDef() override {
		return DEF_;
	}
};

