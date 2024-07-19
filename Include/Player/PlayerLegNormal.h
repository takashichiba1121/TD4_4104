#pragma once
#include"PlayerLeg.h"
class PlayerLegNormal :
    public PlayerLeg
{


	void Initialize(Vector2* playerPos,bool* direction,float* changeAcl) override;

	void Move(bool DirBOTTOM,bool isAttack) override;

	void JumpStart() override;

	void Jump() override;

	void EvasionRoll() override;

	void Falling() override;

	void Draw() override;

	void Load() override;

private:

	Vector2* playerVelocity_;

	bool* direction_;

	float* changeAcl_;

	bool onGround_;

	bool isJump_;

	bool isEvasionRoll_;

#pragma region ステータス

//移動

	float topSpeed_ = 0;

	float acceleration_ = 0;

	float airAcceleration_ = 0;

	float deccelaration_ = 1;

	float airDeccelaration_ = 0;

	float evasionRollSpeed_=16;

	//戦闘

	//ジャンプ

	float gravityAcceleration_ = 0;

	float jumpAcceleration_ = 0;

	float jumpInitialVelocity_ = 0;

	bool isDirBottom_;

#pragma endregion
};

