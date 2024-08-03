#include "DxlibInclude.h"
#include<TextureManager.h>
#include<SoundManager.h>

int LoadGraph(const std::string& FileName)
{
    return TextureManager::Load(FileName);
}

int LoadSoundMem(const std::string& FileName)
{
    return SoundManager::Load(FileName);
}
