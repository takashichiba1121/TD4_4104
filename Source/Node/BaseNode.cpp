#include "BaseNode.h"
#include<cassert>
#include<NodeManager.h>

void BaseNode::SetNode(Node* node)
{
	assert(node);

	position_ = node->position;
	row_ = node->row;
	column_ = node->column;
	type_ = node->type;
	nextDoorsNum_ = node->nextDoorsNum;
}

void BaseNode::SetMapChip(MapChip* mapChip)
{
	mapChip_ = mapChip;
}

void BaseNode::SetPlayer(Player* player)
{
	player_ = player;
}

void BaseNode::SetNodeManagerr(NodeManager* nodeManager)
{
	nodeManager_ = nodeManager;
}
