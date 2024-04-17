#pragma once
#include<stdint.h>
class Input
{
#pragma region Singleton
private:
	Input() {};

	~Input() {};
public:
	Input(const Input& carManager) = delete;

	Input& operator=(const Input& carManager) = delete;

	static Input* Instance();
#pragma endregion

public:
	/// <summary>
	/// 毎フレームやるキーの入力情報の更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーを押しているか
	/// </summary>
	bool PushKey(uint16_t keyNumber);

	/// <summary>
	/// キーを押した瞬間
	/// </summary>
	bool TriggerKey(uint16_t keyNumber);
	/// <summary>
	/// キーを離した瞬間
	/// </summary>
	bool ReleaseKey(uint16_t keyNumber);

private:
	char key[ 256 ] = { 0 };

	char oldKey[ 256 ] = { 0 };
};

