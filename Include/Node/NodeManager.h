#pragma once

#include<array>
#include<random>
#include<memory>
#include<list>
#include<NodeType.h>
#include<Vector2.h>
#include<BaseNode.h>
#include<MapChip.h>

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
	int32_t nodeProbabilities[ NodeType::TYPE_NUM ] = {};

	const int32_t STARTNODE_DREW_MAX_Y = 650;
	const int32_t NODE_DREW_MIN_Y = 100;

	int32_t oldRand = 0;
	int32_t oldRandomJ = 0;
	float scrollSpeed_ = 7.0f;

	int32_t reinforcementImg;
	int32_t transactionImg;
	int32_t battleImg;
	int32_t shopImg;
	int32_t healingImg;
	int32_t startImg;
	int32_t backGroundImg;

	int32_t playerNodePos;

	float leftBottomX;
	float leftBottomY;


	// 遒ｺ邇・・蟶・ｒ菴懈・縺吶ｋ
	std::discrete_distribution<int> distribution;
	std::array<std::unique_ptr<BaseNode>,NodeType::TYPE_NUM - 3>rooms_;
	std::list<Node*> drawNode_;

	
	BaseNode* node_ = nullptr;
	Node* nextNode_ = nullptr;
	Node* selectNode_ = nullptr;

	std::vector<std::vector<Node>>nodes_;
	std::vector<Node*>startNodes_;

	Node bossNode_;
	std::unique_ptr<BaseNode>bossRoom_;

	MapChip* mapChip_;
	Player* player_;
	PowerUpCave* powerUp_;

	bool isNodeDraw;

public:

	static NodeManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();
	void Reset();

	void NodeMapDraw();

	void NodeDrew(int32_t leftBottomX,int32_t leftBottomY,const Node& node,bool line);

	void ChangeNode(size_t doorNo);
	void StartNodeSet(size_t nodeNo);

	void SetMapChip(MapChip* mapChip);
	void SetPlayer(Player* player);
	void SetPowerUp(PowerUpCave* powerUp);

private:

	void GenerateInitialGrid();
	std::vector<int32_t> GetRandomStartingPoints();
	int32_t SetupConnection(int32_t i,int32_t j);
	bool WouldCrossExistingPath(int32_t i,int32_t j,Node* room);
	void SetupRoomTypes();
	void SetRoomRandomly(Node* roomToSet);
	bool RoomHasParentOfType(Node* room,NodeType type);

	NodeManager() = default;
	~NodeManager() = default;
};