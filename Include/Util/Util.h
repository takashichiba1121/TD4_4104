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