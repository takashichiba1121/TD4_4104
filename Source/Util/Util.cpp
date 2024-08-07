#include "Util.h"


using namespace std;

double Lerp(double startPos,double endPos,double maxTime,double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time + startPos;
}

double Lerp(double startPos,double endPos,double time)
{
	double change = endPos - startPos;
	return change * time + startPos;
}
double InQuad(double startPos,double endPos,double maxTime,double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time * time + startPos;
}

float InQuad(float startPos,float endPos,int32_t maxTime,int32_t time)
{
	float _time = static_cast< float >( time ) / static_cast< float >( maxTime );
	float change = endPos - startPos;
	return change * _time * _time + startPos;
}



double OutQuad(double startPos,double endPos,double maxTime,double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return -change * time * ( time - 2 ) + startPos;
}
double InOutQuad(double startPos,double endPos,double maxTime,double time)
{
	time /= maxTime / 2;
	double change = endPos - startPos;
	if ( time < 1 ) return change / 2 * time * time + startPos;
	return -change / 2 * ( ( --time ) * ( time - 2 ) - 1 ) + startPos;
}
double OutInRelayQuad(double startPos,double endPos,double relayPos,double maxTime,double relayTime,double time)
{
	if ( time < relayTime )
	{
		return OutQuad(relayPos,startPos,relayTime,time);
	}
	else
	{
		return InQuad(endPos,relayPos,maxTime - relayTime,time - relayTime);
	}
}
double OutOutRelayQuad(double startPos,double endPos,double relayPos,double maxTime,double relayTime,double time)
{
	if ( time < relayTime )
	{
		return OutQuad(relayPos,startPos,relayTime,time);
	}
	else
	{
		return OutQuad(endPos,relayPos,maxTime - relayTime,time - relayTime);
	}
}
double EaseInBack(double startPos,double endPos,double maxTime,double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.70158;
	double c3 = c1 + 1;
	return change * ( c3 * time * time * time - c1 * time * time ) + startPos;
}
double EaseInBack2(double startPos,double endPos,double maxTime,double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.2;
	double c3 = c1 + 0.8;
	return change * ( c3 * time * time * time - c1 * time * time ) + startPos;
}
double EaseOutBounce(double startPos,double endPos,double time,double maxTime)
{
	time /= maxTime;
	double change = endPos - startPos;
	double n1 = 7.5625;
	double d1 = 2.75;

	if ( time < 1 / d1 )
	{
		return n1 * time * time + startPos + change;
	}
	else if ( time < 2 / d1 )
	{
		return n1 * ( time -= 1.5 / d1 ) * time + 0.75 + startPos + change;
	}
	else if ( time < 2.5 / d1 )
	{
		return n1 * ( time -= 2.25 / d1 ) * time + 0.9375 + startPos + change;
	}
	else
	{
		return n1 * ( time -= 2.625 / d1 ) * time + 0.984375 + startPos + change;
	}
}

int32_t sign(float num)
{
	if ( num > 0 )
	{
		return 1;
	}
	else if ( num < 0 )
	{
		return -1;
	}
	return 0;
}



std::wstring StringToWString(std::string oString)
{
	// SJIS → wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP,0,oString.c_str()
		,-1,( wchar_t* ) NULL,0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[ iBufferSize ];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP,0,oString.c_str(),-1,cpUCS2
		,iBufferSize);

	// stringの生成
	std::wstring oRet(cpUCS2,cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[ ] cpUCS2;

	// 変換結果を返す
	return( oRet );
}



void Counter::ReSetCount()
{
	nowCount_ = 0;
}

void Counter::SetEndCount(float endCount)
{
	nowCount_ = 0;
	endCount_ = endCount;
}

void Counter::SetNowCount(float count)
{
	nowCount_ = count;
}

bool Counter::IsCountUnderZero()
{
	if ( nowCount_ < 0 )
	{
		return true;
	}
	return false;
}

bool Counter::IsCountZero()
{
	if ( nowCount_ == 0 )
	{
		return true;
	}
	return false;
}

bool Counter::IsCountEnd()
{
	if ( nowCount_ > endCount_ )
	{
		return true;
	}
	return false;
}

void Counter::CountDown(float down)
{
	nowCount_ -= down;
}

void Counter::CountUp(float up)
{
	if ( !IsCountEnd() )
	{
		nowCount_ += up;
	}
}

float Counter::GetCount()
{
	return nowCount_;
}

float Counter::GetEndCount()
{
	return endCount_;
}
