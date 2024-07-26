#include"FPS.h"

void FPS::FpsControlBegin()
{
	//周波数取得
	QueryPerformanceFrequency(&cpuClock);
	//計測開始時間の初期化
	QueryPerformanceCounter(&timeStart);
}

void FPS::FpsControlEnd()
{
	//今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	//経過時間
	float lElapsedFrame = static_cast< float >( timeEnd.QuadPart - timeStart.QuadPart ) / static_cast< float >( cpuClock.QuadPart );
	//余裕があるときは待つ
	if ( lElapsedFrame < frameTime )
	{
		//sleep時間
		DWORD sleepTime = static_cast< DWORD >( ( frameTime - lElapsedFrame ) * 1000.0f );
		timeBeginPeriod(1);
		//寝る
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	fps = 1 / lElapsedFrame;

	QueryPerformanceCounter(&timeEnd);
	deltaTime = static_cast< float >( timeEnd.QuadPart - timeStart.QuadPart ) / static_cast< float >( cpuClock.QuadPart );
}

void FPS::SetFrameRate(float frameRate_)
{
	frameTime = 1.0f / frameRate_;
}

float FPS::GetFrameRate()
{
	return fps;
}

float FPS::GetDeltaTime()
{
	return deltaTime;

}
