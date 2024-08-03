#pragma once
#include<cstdint>

enum ChipIndex : uint8_t
{
	NONE,//なし
	ROAD,//道
	DOOR,//使わない
	ROOM,//使わない
	LOCK_ROOM,//使わない
	WALL,//壁
	NEXT,//次
	START,//最初
	FLY_RANGE_ENEMY,//飛ぶ
	LONG_RANGE_ENEMY,//遠距離
	SHORT_RANGE_ENEMY,//近距離
	RANDOM_ENEMY,//ランダム
};