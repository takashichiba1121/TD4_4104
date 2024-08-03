#include "DxlibInclude.h"
#include<TextureManager.h>

int LoadGraph(const std::string& FileName)
{
    return TextureManager::Load(FileName);
}
