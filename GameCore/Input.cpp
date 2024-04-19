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
	memcpy(oldKey_,key_,sizeof(key_));

	GetHitKeyStateAll(key_);
}

bool Input::PushKey(uint16_t keyNumber)
{
		//指定キーを押していればtrueを返す
	if ( key_[ keyNumber ] )
	{
		return true;
	}
	//そうでなければfalse返す
	return false;
}

bool Input::TriggerKey(uint16_t keyNumber)
{
	//指定キーを押していればtrueを返す
	if ( key_[ keyNumber ] && oldKey_[ keyNumber ] == 0 )
	{
		return true;
	}
	//そうでなければfalse返す
	return false;
}

bool Input::ReleaseKey(uint16_t keyNumber)
{
	//指定キーを押していればtrueを返す
	if ( key_[ keyNumber ]==0 && oldKey_[ keyNumber ] )
	{
		return true;
	}
	//そうでなければfalse返す
	return false;
}
