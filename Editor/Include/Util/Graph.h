#pragma once


#include<d3d11.h>
#include<stdint.h>

#include<Vec2.h>

struct Graph
{
	int32_t handle;
	ID3D11ShaderResourceView* pSRV = nullptr;
};

ID3D11ShaderResourceView* GetImageResource11(int softimageHandle);

Graph Load(const char* filepath);

struct UV
{
	float2 min;
	float2 max;
};
