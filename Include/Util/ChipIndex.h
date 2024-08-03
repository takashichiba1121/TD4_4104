#pragma once
#include<cstdint>

enum ChipIndex : uint8_t
{
	NONE,//なし
	ROAD,//道
	WALL,//壁
	SHORT_RANGE_ENEMY,//近距離
	LONG_RANGE_ENEMY,//遠距離
	FLY_RANGE_ENEMY,//飛ぶ
	NEXT,//次
	START,//プレイヤー初期位置
	RANDOM_ENEMY,//ランダム
	DOOR,//使わない
	ROOM,//使わない
	LOCK_ROOM,//使わない
};