#pragma once
#include<BaseNode.h>
#include<map>

class NodeManager
{
private:

	static constexpr int X_DIST = 30;
	static constexpr int Y_DIST = 25;
	static constexpr int PLACEMENT_RANDOMNESS = 5;
	static constexpr int FLOORS = 15;
	static constexpr int MAP_WIDTH = 7;
	static constexpr int PATHS = 6;
	static constexpr float MONSTER_ROOM_WEIGHT = 10.0;
	static constexpr float SHOP_ROOM_WEIGHT = 2.5;
	static constexpr float CAMPFIRE_ROOM_WEIGHT = 4.0;

	std::map<BaseNode::Type,float> randomRoomTypeWeights = {
	{BaseNode::MONSTER, 0.0},
	{BaseNode::CAMPFIRE, 0.0},
	{BaseNode::SHOP, 0.0}
	};
	float randomRoomTypeTotalWeight = 0;

	BaseNode* node_;
	BaseNode* nextNode_;

	std::vector<std::vector<BaseNode*>>nodes_;

public:

	static NodeManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	void ChangeNodeSelectLeft();
	void ChangeNodeSelectCenter();
	void ChangeNodeSelectRight();

private:

	void GenerateInitialGrid();
	std::vector<int> GetRandomStartingPoints();
	int SetupConnection(int i,int j);
	bool WouldCrossExistingPath(int i,int j,BaseNode* room);
	void SetupBossRoom();
	void SetupRandomRoomWeights();
	void SetupRoomTypes();

	NodeManager() = default;
	~NodeManager() = default;
};

