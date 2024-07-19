#pragma once

#include<array>
#include<random>
#include<memory>
#include<list>
#include<NodeType.h>
#include<Vector2.h>
#include<BaseNode.h>
#include<MapChip.h>

class NodeManager
{
private:

	int32_t X_DIST = 0;
	int32_t Y_DIST = 0;
	int32_t PLACEMENT_RANDOMNESS = 0;
	int32_t FLOORS = 0;
	int32_t MAP_WIDTH =0;
	int32_t PATHS =0;
	int32_t START_POINT =0;

	int oldRand = 0;
	int oldRandomJ = 0;

	int32_t reinforcementImg;
	int32_t transactionImg;
	int32_t battleImg;
	int32_t shopImg;
	int32_t healingImg;
	int32_t startImg;

	int32_t nodeProbabilities[ NodeType::TYPE_NUM ] = {};

	// 確率分布を作成する
	std::discrete_distribution<int> distribution;
	std::array<std::unique_ptr<BaseNode>,NodeType::TYPE_NUM - 2>rooms_;
	std::list<Node*> drawNode_;

	BaseNode* node_ = nullptr;
	Node* nextNode_ = nullptr;
	Node* selectNode_ = nullptr;

	std::vector<std::vector<Node>>nodes_;
	std::vector<Node*>startNodes_;

	MapChip* mapChip_;
	Player* player_;

public:

	static NodeManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();
	void Reset();

	void NodeDrew(int32_t leftBottomX,int32_t leftBottomY);

	void ChangeNode(size_t doorNo);
	void StartNodeSet(size_t nodeNo);

	void SetMapChip(MapChip* mapChip);
	void SetPlayer(Player* player);

private:

	void GenerateInitialGrid();
	std::vector<int> GetRandomStartingPoints();
	int SetupConnection(int i,int j);
	bool WouldCrossExistingPath(int i,int j,Node* room);
	void SetupRoomTypes();
	void SetRoomRandomly(Node* roomToSet);
	bool RoomHasParentOfType(Node* room,NodeType type);

	NodeManager() = default;
	~NodeManager() = default;
};

struct Node
{
	NodeType type;
	Vector2 position;
	int32_t row = 0;
	int32_t column = 0;
	int32_t nextDoorsNum = 0;

	std::vector<Node*> previews;
	std::vector<Node*>nexts;
};