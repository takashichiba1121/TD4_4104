#include"DxlibInclude.h"
#include "Input.h"
#include<string>

Input* Input::Instance()
{
	static Input instance;

	return &instance;
}

void Input::Update()
{
		//前回のキー入力を保存
	memcpy(oldKey,key,sizeof(key));

	GetHitKeyStateAll(key);
}

bool Input::PushKey(uint16_t keyNumber)
{
		//指定キーを押していればtrueを返す
	if ( key[ keyNumber ] )
	{
		return true;
	}
	//そうでなければfalse返す
	return false;
}

bool Input::TriggerKey(uint16_t keyNumber)
{
	//指定キーを押していればtrueを返す
	if ( key[ keyNumber ] && oldKey[ keyNumber ] == 0 )
	{
		return true;
	}
	//そうでなければfalse返す
	return false;
}

bool Input::ReleaseKey(uint16_t keyNumber)
{
	//指定キーを押していればtrueを返す
	if ( key[ keyNumber ]==0 && oldKey[ keyNumber ] )
	{
		return true;
	}
	//そうでなければfalse返す
	return false;
}
