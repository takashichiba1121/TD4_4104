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
	void Update();

	/// <summary>
	/// キーを押しているか？
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PushKey(uint16_t keyNumber);

	bool TriggerKey(uint16_t keyNumber);

	bool ReleaseKey(uint16_t keyNumber);

private:
	char key[ 256 ] = { 0 };;

	char oldKey[ 256 ] = { 0 };;
};

