#pragma once
#include <stdint.h>

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

enum Effects
{
	BURN,
	BIND,
	DELAY,
	ICED,
	CURSE
};

class Counter
{
private:
	int32_t nowCount_;
	int32_t endCount_;
public:
	void CountUp(uint32_t up = 1);
	void CountDown(uint32_t down = 1);
	bool IsCountEnd();
	bool IsCountZero();
	bool IsCountUnderZero();
	void SetEndCount(int32_t endCount);
	void ReSetCount();
	int32_t GetCount();
	int32_t GetEndCount();

};