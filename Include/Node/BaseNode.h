#pragma once

#include<Vector2.h>
#include<NodeType.h>
#include<cstdint>
#include<MapChip.h>
#include<Player.h>

struct Node;
class NodeManager;


class BaseNode
{
protected:

	Vector2 position_;
	int32_t row_;
	int32_t column_;
	NodeType type_;
	uint32_t nextDoorsNum_;

	MapChip* mapChip_;
	Player* player_;
	NodeManager* nodeManager_;

	struct NextDoor
	{
		Vector2 pos;
		int32_t id;
	};

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Reset() = 0;
	virtual void Finalize() = 0;
	virtual Vector2 GetPlayerStartPos() = 0;

	void SetNode(Node* node);
	void SetMapChip(MapChip* mapChip);
	void SetPlayer(Player* player);
	void SetNodeManagerr(NodeManager* nodeManager);

	BaseNode() = default;
	virtual ~BaseNode() = default;
};
