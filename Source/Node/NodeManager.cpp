#include "NodeManager.h"
#include<algorithm>
#include<DxlibInclude.h>
#include"Vector2.h"

#include<ReinforcementNode.h>
#include<TransactionNode.h>
#include<BattleNode.h>
#include<ShopNode.h>
#include<HealingNode.h>
#include<StartNode.h>
#include<BossNode.h>

#include<GameConfig.h>
#include<Input.h>

int GetRand(int min_,int max_)
{
	// 乱数生成器
	static std::random_device slRD;
	static std::default_random_engine lsEngine(slRD());

	// 分布生成器
	std::uniform_int_distribution<int> lsDistribution(min_,max_);

	// 乱数を生成
	return lsDistribution(lsEngine);
}

NodeManager* NodeManager::GetInstance()
{
	static NodeManager instance;
	return &instance;
}

void NodeManager::Initialize()
{
	{
		GameConfig::Node* config = GameConfig::GetNodeConfig();
		X_DIST = config->xDistance;
		Y_DIST = config->yDistance;
		PLACEMENT_RANDOMNESS = config->placementRandomness;
		FLOORS = config->floors;
		MAP_WIDTH = config->width;
		PATHS = config->paths;
		START_POINT = config->startPoints;

		for ( size_t i = 0; i < config->nodeProbabilities.size(); i++ )
		{
			nodeProbabilities[ i ] = config->nodeProbabilities[ i ];
		}
	}

	reinforcementImg = LoadGraph("Resources/Node/reinforcement.png");
	transactionImg = LoadGraph("Resources/Node/transaction.png");
	battleImg = LoadGraph("Resources/Node/battle.png");
	shopImg = LoadGraph("Resources/Node/shop.png");
	healingImg = LoadGraph("Resources/Node/healing.png");
	startImg = LoadGraph("Resources/Node/start.png");
	backGroundImg = LoadGraph("Resources/BackGround/mapBackGround.png");

	distribution = std::discrete_distribution<int>(nodeProbabilities,nodeProbabilities + NodeType::TYPE_NUM);

	GenerateInitialGrid();


	std::vector<int> startingPoints = GetRandomStartingPoints();

	for ( int k = 0; k < PATHS; k++ )
	{
		int j = startingPoints[ DxLib::GetRand(startingPoints.size() - 1) ];

		int current_j = j;
		for ( int i = 0; i < FLOORS - 1; ++i )
		{
			current_j = SetupConnection(i,current_j);
		}
	}

	for ( int point : startingPoints )
	{
		if ( nodes_[ 0 ][ point ].type.value != NodeType::Type::NO_CHILDREN )
		{
			startNodes_.push_back(&nodes_[ 0 ][ point ]);
			drawNode_.push_back(&nodes_[ 0 ][ point ]);
			nodes_[ 0 ][ point ].type.value = NodeType::START;
		}
	}

	for ( int i = 0; i < MAP_WIDTH; ++i )
	{
		if ( !nodes_[ FLOORS - 1 ][ i ].previews.empty() )
		{
			nodes_[ FLOORS - 1 ][ i ].nexts.push_back(&bossNode_);
			nodes_[ FLOORS - 1 ][ i ].type.value = NodeType::Type::NONE;
		}
	}

	SetupRoomTypes();

	for ( auto& node : drawNode_ )
	{
		std::vector<Node*>& nodes = node->nexts;
		std::sort(nodes.begin(),nodes.end());
	}

	std::sort(startNodes_.begin(),startNodes_.end());

	rooms_[ NodeType::REINFORCEMENT ] = std::make_unique<ReinforcementNode>();
	rooms_[ NodeType::TRANSACTION ] = std::make_unique<TransactionNode>();
	rooms_[ NodeType::BATTLE ] = std::make_unique<BattleNode>();
	rooms_[ NodeType::SHOP ] = std::make_unique<ShopNode>();
	rooms_[ NodeType::HEALING ] = std::make_unique<HealingNode>();
	rooms_[ NodeType::START ] = std::make_unique<StartNode>();

	bossRoom_ = std::make_unique<BossNode>();

	for ( auto& room : rooms_ )
	{
		room->SetMapChip(mapChip_);
		room->SetPlayer(player_);
		room->SetNodeManagerr(this);
		room->Initialize();
	}

	bossRoom_->SetMapChip(mapChip_);
	bossRoom_->SetPlayer(player_);
	bossRoom_->SetNodeManagerr(this);
	bossRoom_->Initialize();

	bossNode_.type.value = NodeType::BOSS;

	playerNodePos = 0;
	leftBottomX = 450;
	leftBottomY = 650;

	int32_t bossNodeX = 0;
	int32_t nodeCount = 0;

	for ( size_t i = 0; i < MAP_WIDTH; i++ )
	{
		if ( !nodes_[ FLOORS - 1 ][ i ].previews.empty() )
		{
			nodeCount++;
			bossNodeX += nodes_[ FLOORS - 1 ][ i ].position.x;

			if ( bossNode_.position.y > nodes_[ FLOORS - 1 ][ i ].position.y )
			{
				bossNode_.position.y = nodes_[ FLOORS - 1 ][ i ].position.y;
			}
		}
	}

	bossNode_.position.y -= Y_DIST;
	bossNode_.position.x = bossNodeX / nodeCount;
	bossNode_.row = FLOORS;
}

void NodeManager::Update()
{
	if ( isNodeDraw )
	{
		int32_t scroll = GetMouseWheelRotVol();
		if ( scroll == -1 && leftBottomY > STARTNODE_DREW_MAX_Y )
		{
			leftBottomY += 1.0f * scroll;
		}

		if ( scroll == 1 && nodes_[ playerNodePos - 1 ][ 0 ].position.y + leftBottomY < NODE_DREW_MIN_Y )
		{
			leftBottomY += 1.0f * scroll;
		}
	}

	if ( nextNode_ )
	{
		if ( node_ )
		{
			node_->Finalize();
		}

		if ( nextNode_->type.value == NodeType::BOSS )
		{
			node_ = bossRoom_.get();
		}
		else
		{
			node_ = rooms_[ nextNode_->type.value ].get();

		}

		node_->SetNode(nextNode_);
		node_->Reset();

		selectNode_ = nextNode_;
		nextNode_ = nullptr;
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_M) )
	{
		if ( isNodeDraw )
		{
			isNodeDraw = false;
		}
		else
		{
			isNodeDraw = true;
		}
	}

	node_->Update();
}

void NodeManager::Draw()
{
	if ( isNodeDraw )
	{
		NodeMapDraw();
	}

	node_->Draw();
}

void NodeManager::Reset()
{
	for ( int i = 0; i < FLOORS; ++i )
	{
		for ( int j = 0; j < MAP_WIDTH; ++j )
		{
			nodes_[ i ][ j ].nexts.clear();
			nodes_[ i ][ j ].previews.clear();
			nodes_[ i ][ j ].nextDoorsNum = 0;
		}
	}

	drawNode_.clear();

	std::vector<int> startingPoints = GetRandomStartingPoints();

	for ( int k = 0; k < PATHS; k++ )
	{
		int j = startingPoints[ DxLib::GetRand(startingPoints.size() - 1) ];

		int current_j = j;
		for ( int i = 0; i < FLOORS - 1; ++i )
		{
			current_j = SetupConnection(i,current_j);

		}
	}

	startNodes_.clear();

	for ( int point : startingPoints )
	{
		if ( nodes_[ 0 ][ point ].type.value != NodeType::Type::NO_CHILDREN )
		{
			startNodes_.push_back(&nodes_[ 0 ][ point ]);
			nodes_[ 0 ][ point ].type.value = NodeType::START;
			drawNode_.push_back(&nodes_[ 0 ][ point ]);
		}
	}

	for ( auto& node : drawNode_ )
	{
		std::vector<Node*>& nodes = node->nexts;
		std::sort(nodes.begin(),nodes.end());
	}

	std::sort(startNodes_.begin(),startNodes_.end());

}

void NodeManager::NodeMapDraw()
{

	playerNodePos = selectNode_->row + 3;
	playerNodePos = min(playerNodePos,FLOORS);

	DrawRotaGraph(GameConfig::GetWindowWidth()/2,GameConfig::GetWindowHeight()/2-2,1.0f,0.0,backGroundImg,true);

	DrawCircle(leftBottomX + selectNode_->position.x,leftBottomY + selectNode_->position.y,20,GetColor(255,0,0));
	for ( int i = 0; i < playerNodePos; ++i )
	{
		for ( int j = 0; j < MAP_WIDTH; ++j )
		{
			NodeDrew(leftBottomX,leftBottomY,nodes_[ i ][ j ],true);

			if ( playerNodePos == FLOORS )
			{
				NodeDrew(leftBottomX,leftBottomY,bossNode_,true);
			}
		}
	}

}

void NodeManager::NodeDrew(int32_t leftBottomX,int32_t leftBottomY,const Node& node,bool line)
{
	if ( line )
	{
		for ( size_t k = 0; k < node.nexts.size(); k++ )
		{
			DrawLine(leftBottomX + node.position.x,leftBottomY + node.position.y,leftBottomX + node.nexts[ k ]->position.x,leftBottomY + node.nexts[ k ]->position.y,GetColor(255,255,255));
		}
	}

	switch ( node.type.value )
	{
	case NodeType::Type::REINFORCEMENT:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,reinforcementImg,true);
		break;
	case NodeType::Type::TRANSACTION:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,transactionImg,true);
		break;
	case NodeType::Type::BATTLE:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,battleImg,true);
		break;
	case NodeType::Type::SHOP:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,shopImg,true);
		break;
	case NodeType::Type::HEALING:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,healingImg,true);
		break;
	case NodeType::Type::START:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,startImg,true);
		break;
	case NodeType::Type::BOSS:
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,0.5,0,startImg,true);
		break;
	default:
		break;
	}
}

void NodeManager::ChangeNode(size_t doorNo)
{
	assert(doorNo < selectNode_->nexts.size());
	nextNode_ = selectNode_->nexts[ doorNo ];
}

void NodeManager::StartNodeSet(size_t nodeNo)
{
	assert(nodeNo < startNodes_.size());
	nextNode_ = startNodes_[ nodeNo ];
}

void NodeManager::SetMapChip(MapChip* mapChip)
{
	mapChip_ = mapChip;
}

void NodeManager::SetPlayer(Player* player)
{
	player_ = player;
}

void NodeManager::GenerateInitialGrid()
{
	for ( int i = 0; i < FLOORS; ++i )
	{
		std::vector<Node> adjacentRooms;

		for ( int j = 0; j < MAP_WIDTH; ++j )
		{
			Node currentRoom;
			Vector2 offset(DxLib::GetRand(PLACEMENT_RANDOMNESS),DxLib::GetRand(PLACEMENT_RANDOMNESS));
			currentRoom.position = Vector2(j * X_DIST,i * -Y_DIST) + offset;
			currentRoom.row = i;
			currentRoom.column = j;
			currentRoom.type.value = NodeType::NO_CHILDREN;

			if ( i == FLOORS - 1 )
			{
				currentRoom.position.y = i * -Y_DIST;
			}

			adjacentRooms.push_back(currentRoom);
		}

		nodes_.push_back(adjacentRooms);
	}
}

std::vector<int> NodeManager::GetRandomStartingPoints()
{
	std::vector<int> yCoordinates;
	int uniquePoints = 0;

	while ( uniquePoints < 2 )
	{
		uniquePoints = 0;
		yCoordinates.clear();

		for ( int i = 0; i < START_POINT; ++i )
		{
			int startingPoint = DxLib::GetRand(MAP_WIDTH - 1);

			if ( find(yCoordinates.begin(),yCoordinates.end(),startingPoint) == yCoordinates.end() )
			{
				uniquePoints++;
			}

			yCoordinates.push_back(startingPoint);
		}
	}

	return yCoordinates;
}

int NodeManager::SetupConnection(int i,int j)
{
	Node* nextRoom = nullptr;
	Node* currentRoom = &nodes_[ i ][ j ];
	int rand_ = 0;
	int selectJ;
	int random_j = 0;

	{
		oldRandomJ = random_j;
		while ( random_j == oldRandomJ )
		{
			oldRandomJ = random_j;
			rand_ = GetRand(-1,1);
			selectJ = rand_ + j;
			random_j = std::clamp(selectJ,0,MAP_WIDTH - 1);
		}

		nextRoom = &nodes_[ i + 1 ][ random_j ];

		if ( WouldCrossExistingPath(i,j,nextRoom) )
		{
			rand_ *= -1;
			selectJ = rand_ + j;
			random_j = std::clamp(selectJ,0,MAP_WIDTH - 1);
			nextRoom = &nodes_[ i + 1 ][ random_j ];

			if ( WouldCrossExistingPath(i,j,nextRoom) )
			{
				oldRandomJ = random_j;
				while ( random_j == oldRandomJ )
				{
					oldRandomJ = random_j;
					rand_ = GetRand(-1,1);
					selectJ = rand_ + j;
					random_j = std::clamp(selectJ,0,MAP_WIDTH - 1);
				}

				nextRoom = &nodes_[ i + 1 ][ random_j ];
			}
		}
	}

	if ( std::find_if(currentRoom->nexts.begin(),currentRoom->nexts.end(),[ & ] (Node* node)
		{
			return node->column == nextRoom->column && node->row == nextRoom->row;
		}) == currentRoom->nexts.end() )
	{
		currentRoom->nexts.push_back(nextRoom);
		currentRoom->nextDoorsNum += 1;
		nextRoom->previews.push_back(currentRoom);
		currentRoom->type.value = NodeType::NONE;
	}

	return nextRoom->column;
}

bool NodeManager::WouldCrossExistingPath(int i,int j,Node* room)
{
	Node* leftNeighbour = nullptr;
	Node* rightNeighbour = nullptr;

	if ( j > 0 )
	{
		leftNeighbour = &nodes_[ i ][ j - 1 ];
	}
	if ( j < MAP_WIDTH - 1 )
	{
		rightNeighbour = &nodes_[ i ][ j + 1 ];
	}

	if ( rightNeighbour && room->column > j )
	{
		for ( Node* nextRoom : rightNeighbour->nexts )
		{
			if ( nextRoom->column < room->column )
			{
				return true;
			}
		}
	}

	if ( leftNeighbour && room->column < j )
	{
		for ( Node* next_room : leftNeighbour->nexts )
		{
			if ( next_room->column > room->column )
			{
				return true;
			}
		}
	}

	return false;
}

void NodeManager::SetupRoomTypes()
{
	for ( auto& current_floor : nodes_ )
	{
		for ( Node& room : current_floor )
		{
			if ( room.type.value == NodeType::NONE )
			{
				SetRoomRandomly(&room);
			}
		}
	}
}

void NodeManager::SetRoomRandomly(Node* roomToSet)
{
	static std::mt19937 gen(std::time(nullptr));
	roomToSet->type.value = static_cast< NodeType::Type >( distribution(gen) );

}

bool NodeManager::RoomHasParentOfType(Node* room,NodeType type)
{
	std::vector<Node*> parents;

	if ( room->column > 0 && room->row > 0 )
	{
		Node* parentCandidate = &nodes_[ room->row - 1 ][ room->column - 1 ];
		if ( find(parentCandidate->nexts.begin(),parentCandidate->nexts.end(),room) != parentCandidate->nexts.end() )
		{
			parents.push_back(parentCandidate);
		}
	}
	if ( room->row > 0 )
	{
		Node* parentCandidate = &nodes_[ room->row - 1 ][ room->column ];
		if ( find(parentCandidate->nexts.begin(),parentCandidate->nexts.end(),room) != parentCandidate->nexts.end() )
		{
			parents.push_back(parentCandidate);
		}
	}
	if ( room->column < MAP_WIDTH - 1 && room->row > 0 )
	{
		Node* parentCandidate = &nodes_[ room->row - 1 ][ room->column + 1 ];
		if ( find(parentCandidate->nexts.begin(),parentCandidate->nexts.end(),room) != parentCandidate->nexts.end() )
		{
			parents.push_back(parentCandidate);
		}
	}

	for ( Node* parent : parents )
	{
		if ( parent->type.value == type.value )
		{
			return true;
		}
	}

	return false;
}