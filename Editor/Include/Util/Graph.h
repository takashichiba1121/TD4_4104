#pragma once


#include<d3d11.h>
#include<stdint.h>

struct Graph
{
	int32_t handle;
	ID3D11ShaderResourceView* pSRV_ = nullptr;
};

static ID3D11ShaderResourceView* GetImageResource11(int softimageHandle);

Graph LoadGraph(char* filepath);
