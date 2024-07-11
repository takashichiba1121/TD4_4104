#pragma once
#include<BaseNode.h>
#include<map>
#include<random>

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
	static constexpr float MONSTER_ROOM_WEIGHT = 10.0;
	static constexpr float SHOP_ROOM_WEIGHT = 2.5;
	static constexpr float CAMPFIRE_ROOM_WEIGHT = 4.0;

	int oldRand = 0;
	int oldRandomJ = 0;

	const int probabilities[ BaseNode::Type::TYPE_NUM ] = { 20, 30, 10, 15, 25 ,0}; // A: 20%, B: 30%, C: 10%, D: 15%, E: 25%

	// 確率分布を作成する
	std::discrete_distribution<int> distribution;


	BaseNode* node_;
	BaseNode* nextNode_;

	std::vector<std::vector<BaseNode*>>nodes_;

public:

	static NodeManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();
	void Reset();

	void ChangeNodeSelectLeft();
	void ChangeNodeSelectCenter();
	void ChangeNodeSelectRight();

	void NodeDrew();

private:

	void GenerateInitialGrid();
	std::vector<int> GetRandomStartingPoints();
	int SetupConnection(int i,int j);
	bool WouldCrossExistingPath(int i,int j,BaseNode* room);
	void SetupRoomTypes();
	void SetRoomRandomly(BaseNode* roomToSet);
	bool RoomHasParentOfType(BaseNode* room,BaseNode::Type type);

	NodeManager() = default;
	~NodeManager() = default;
};

