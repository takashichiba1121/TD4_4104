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

	static constexpr int X_DIST = 30;
	static constexpr int Y_DIST = 25;
	static constexpr int PLACEMENT_RANDOMNESS = 5;
	static constexpr int FLOORS = 15;
	static constexpr int MAP_WIDTH = 7;
	static constexpr int PATHS = 5;
	static constexpr int START_POINT = 3;

	int oldRand = 0;
	int oldRandomJ = 0;

	const int probabilities[ NodeType::TYPE_NUM ] = { 20, 30, 10, 15, 25 ,0,0 }; // A: 20%, B: 30%, C: 10%, D: 15%, E: 25%

	// 確率分布を作成する
	std::discrete_distribution<int> distribution;
	std::array<std::unique_ptr<BaseNode>,NodeType::TYPE_NUM - 1>rooms_;

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

	void NodeDrew();

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