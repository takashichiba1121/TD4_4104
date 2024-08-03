#pragma once
#include <stdint.h>
#include <string>
#include <locale>
#include <codecvt>
#include <Windows.h>
#define  PI 3.14159265358979323846

double Lerp(double startPos,double endPos,double maxTime,double time);
double Lerp(double startPos,double endPos,double time);
double InQuad(double startPos,double endPos,double maxTime,double time);
float InQuad(float startPos,float endPos,int32_t maxTime,int32_t time);
double OutQuad(double startPos,double endPos,double maxTime,double time);
double InOutQuad(double startPos,double endPos,double maxTime,double time);
double OutInRelayQuad(double startPos,double endPos,double relayPos,double maxTime,double relayTime,double time);
double OutOutRelayQuad(double startPos,double endPos,double relayPos,double maxTime,double relayTime,double time);
double EaseInBack(double startPos,double endPos,double maxTime,double time);
double EaseInBack2(double startPos,double endPos,double maxTime,double time);
double EaseOutBounce(double startPos,double endPos,double time,double maxTime);

int32_t sign(float num);

std::wstring StringToWString(std::string oString);

enum Effects
{
	BIND,
	ICED,
	BURN,
	DELAY,
	CURSE,
	END //配列制御のための目印として使用。実際の状態異常では使用しない。
};

class Counter
{
private:
	float nowCount_;
	float endCount_;
public:
	void CountUp(float up = 1);
	void CountDown(float down = 1);
	bool IsCountEnd();
	bool IsCountZero();
	bool IsCountUnderZero();
	void SetEndCount(float endCount);
	void SetNowCount(float count);
	void ReSetCount();
	float GetCount();
	float GetEndCount();

};

constexpr float DEG_TO_RAD = 3.141592f / 180.0f;
