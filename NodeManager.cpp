#include "NodeManager.h"

NodeManager* NodeManager::GetInstance()
{
	static NodeManager instance;
    return &instance;
}

void NodeManager::Initialize()
{
}

void NodeManager::Update()
{
	if ( nextNode_)
	{
		nextNode_->previews_ = node_;
		node_ = nextNode_;

		node_->Initialize();

		nextNode_ = nullptr;
	}

	node_->Update();
}

void NodeManager::Draw()
{
	node_->Draw();
}

void NodeManager::ChangeNodeSelectLeft()
{
	nextNode_ = node_->nexts_[ 0 ];
}

void NodeManager::ChangeNodeSelectCenter()
{
	nextNode_ = node_->nexts_[ 1 ];
}

void NodeManager::ChangeNodeSelectRight()
{
	nextNode_ = node_->nexts_[ 2 ];
}
