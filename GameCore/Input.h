#pragma once
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

	bool PushKey(int keyNumber);

	bool TriggerKey(int keyNumber);

	bool OldKey(int keyNumber);

private:
	char key[ 256 ] = { 0 };;

	char oldKey[ 256 ] = { 0 };;
};

