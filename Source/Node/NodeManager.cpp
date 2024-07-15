#include "NodeManager.h"
#include<cassert>
#include<algorithm>
#include<DxlibInclude.h>
#include"Vector2.h"

#include<ReinforcementNode.h>
#include<TransactionNode.h>
#include<BattleNode.h>
#include<ShopNode.h>
#include<HealingNode.h>
#include<StartNode.h>
#include<GameConfig.h>

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
	distribution = std::discrete_distribution<int>(probabilities,probabilities + NodeType::TYPE_NUM);

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

	distribution = std::discrete_distribution<int>(nodeProbabilities,nodeProbabilities + NodeType::TYPE_NUM);

	GenerateInitialGrid();

	std::vector<int> startingPoints = GetRandomStartingPoints();

	for ( int k = 0; k < PATHS; k++ )
	{
		int j = startingPoints[ GetRand(0,startingPoints.size() - 1) ];

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
	rooms_[ NodeType::NONE ] = std::make_unique<StartNode>();

	for ( auto& room : rooms_ )
	{
		room->SetMapChip(mapChip_);
		room->SetPlayer(player_);
		room->SetNodeManagerr(this);
		room->Initialize();
	}
}

void NodeManager::Update()
{
	if ( nextNode_ )
	{
		if ( node_ )
		{
			node_->Finalize();
		}

		node_ = rooms_[ nextNode_->type.value ].get();
		node_->SetNode(nextNode_);
		node_->Reset();

		selectNode_ = nextNode_;
		nextNode_ = nullptr;
	}

	node_->Update();
}

void NodeManager::Draw()
{
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
		int j = startingPoints[ GetRand(0,startingPoints.size() - 1) ];

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

void NodeManager::NodeDrew()
{
	for ( int i = 0; i < FLOORS; ++i )
	{
		for ( int j = 0; j < MAP_WIDTH; ++j )
		{
			if ( nodes_[ i ][ j ].type.value != NodeType::NO_CHILDREN )
			{
				if ( i == selectNode_->row && j == selectNode_->column )
				{
					DrawCircle(100 + j * 30,( 60 + FLOORS * 30 ) - ( 60 + i * 30 ),3,GetColor(255,0,0));
				}
				else
				{
					DrawCircle(100 + j * 30,( 60 + FLOORS * 30 ) - ( 60 + i * 30 ),3,GetColor(255,255,255));
				}

				for ( size_t k = 0; k < nodes_[ i ][ j ].nexts.size(); k++ )
				{
					DrawLine(100 + j * 30,( 60 + FLOORS * 30 ) - ( 60 + i * 30 ),100 + nodes_[ i ][ j ].nexts[ k ]->column * 30,( 60 + FLOORS * 30 ) - ( 60 + nodes_[ i ][ j ].nexts[ k ]->row * 30 ),GetColor(255,255,255));
				}
			}
		}

	}

	DrawCircle(leftBottomX + selectNode_->position.x,leftBottomY + selectNode_->position.y,3,GetColor(255,0,0));
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
			Vector2 offset(GetRand(0,PLACEMENT_RANDOMNESS),GetRand(0,PLACEMENT_RANDOMNESS));
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
			int startingPoint = GetRand(0,MAP_WIDTH - 1);

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
			for ( Node* next_room : room.nexts )
			{
				if ( next_room->type.value == NodeType::NONE )
				{
					SetRoomRandomly(next_room);
					drawNode_.push_back(next_room);
				}
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

void NodeManager::SetRoomRandomly(Node* roomToSet)
{
	NodeType typeCandidate;
	std::mt19937 gen(std::time(nullptr));
	typeCandidate.value = static_cast< NodeType::Type >( distribution(gen) );
	roomToSet->type = typeCandidate;
}