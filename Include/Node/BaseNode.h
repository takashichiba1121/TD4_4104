#pragma once

#include<Vector2.h>
#include<NodeType.h>
#include<cstdint>
#include<MapChip.h>
#include<Player.h>
#include<PowerUpCave.h>

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
	PowerUpCave* powerUp_;
	DealDaemon* dealer_;
	EnemyManager* enemys_;
	bool* isScrollStop_;

	struct NextDoor
	{
		Vector2 pos;
		int32_t id;
	};

	std::vector<NextDoor> nextdoors_;

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
	void SetPowerUp(PowerUpCave* powerUp);
	void SetDealer(DealDaemon* dealer);
	void SetNodeManagerr(NodeManager* nodeManager);
	void SetEnemyManager(EnemyManager* enemyManager);
	void SetScrollStop(bool* scrollStop);

	void PlayerNodeMove();

	void GetNextDoors();

	BaseNode() = default;
	virtual ~BaseNode() = default;
};
