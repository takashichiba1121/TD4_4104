#pragma once

#include<memory>
#include<Windows.h>

class FPS
{
private:
	LARGE_INTEGER cpuClock{};
	LARGE_INTEGER timeStart{};
	LARGE_INTEGER timeEnd{};
	float frameTime = 1 / 60.0f;
	float fps;
	float deltaTime;

	int32_t PADING;

public:
	void FpsControlBegin();

	void FpsControlEnd();

	void SetFrameRate(float frameRate_);

	float GetFrameRate();

	float GetDeltaTime();

	FPS() = default;
	~FPS() = default;
};