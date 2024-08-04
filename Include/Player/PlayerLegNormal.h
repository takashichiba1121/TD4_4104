#pragma once
#include"PlayerLeg.h"
class PlayerLegNormal :
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

	uint32_t playerStandTexture_;

	uint32_t PlayerStandTextureCount_ = 0;

	uint32_t playerJumpTexture_;

	uint32_t PlayerJumpTextureCount_ = 0;

	uint32_t playerDownTexture_;

	uint32_t PlayerDownTextureCount_ = 0;

	uint32_t playerDushTexture_;

	uint32_t PlayerDushTextureCount_ = 0;

	const uint32_t DEF_ = 70;

	int evasionRollSoundId_;

	int jumpSoundId_;

	int landingSoundId_;

#pragma region ステータス

//移動

	float topSpeed_ = 0;

	float acceleration_ = 0;

	float airAcceleration_ = 0;

	float deccelaration_ = 1;

	float airDeccelaration_ = 0;

	float evasionRollSpeed_=24;

	//戦闘

	//ジャンプ

	float gravityAcceleration_ = 0;

	float jumpAcceleration_ = 0;

	float jumpInitialVelocity_ = 0;

	bool isDirBottom_;

	bool oldIsDirBottom_;

#pragma endregion

public:

	void Initialize(Vector2* playerPos,bool* direction,float* changeAcl) override;

	void Move(bool DirBOTTOM,bool isAttack,const Vector2& pos,const float pow) override;

	void JumpStart() override;

	void Jump() override;

	void EvasionRoll() override;

	void Falling() override;

	void Draw(const Vector2& pos,const Vector2& size,Vector2 scroll) override;

	void Load() override;

	uint32_t GetDef() override {
		return DEF_;
	}

	bool IsEvasionRoll()override
	{
		return isEvasionRoll_;
	}
};

