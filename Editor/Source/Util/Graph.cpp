#include "Graph.h"

#include<DxlibInclude.h>

ID3D11ShaderResourceView* GetImageResource11(int softimageHandle)
{

	ID3D11Device* device = static_cast< ID3D11Device* >( const_cast< void* >( GetUseDirect3D11Device() ) );
	ID3D11DeviceContext* context = static_cast< ID3D11DeviceContext* >( const_cast< void* >( GetUseDirect3D11DeviceContext() ) );

	ID3D11Texture2D* texture = ( ID3D11Texture2D* ) GetGraphID3D11Texture2D(softimageHandle);
	// ShaderResourceViewの作成
	ID3D11ShaderResourceView* pSRV = nullptr;
	device->CreateShaderResourceView(texture,nullptr,&pSRV);

	return pSRV;
}

Graph Load(const char* filepath)
{
	Graph ret;
	ret.handle = LoadGraph(filepath);
	ret.pSRV = GetImageResource11(ret.handle);
	return ret;
}