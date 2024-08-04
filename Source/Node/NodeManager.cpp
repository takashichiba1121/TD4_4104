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
#include<Collision.h>

int32_t GetRand(int32_t min_,int32_t max_)
{
	// 乱数生成器
	static std::random_device slRD;
	static std::default_random_engine lsEngine(slRD());

	// 分布生成器
	std::uniform_int_distribution<int32_t> lsDistribution(min_,max_);

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
	if ( !init_ )
	{
		init_ = true;

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

		reinforcementImg = LoadGraph(std::string("Resources/Node/reinforcement.png"));
		transactionImg = LoadGraph(std::string("Resources/Node/transaction.png"));
		battleImg = LoadGraph(std::string("Resources/Node/battle.png"));
		shopImg = LoadGraph(std::string("Resources/Node/shop.png"));
		healingImg = LoadGraph(std::string("Resources/Node/healing.png"));
		startImg = LoadGraph(std::string("Resources/Node/start.png"));
		bossImg = LoadGraph(std::string("Resources/Node/boss.png"));

		backGroundImg = LoadGraph(std::string("Resources/BackGround/mapBackGround.png"));

		openSound_ = LoadSoundMem(std::string("Resources/Sound/SFX_UI_map_open.mp3"));
		closeSound_ = LoadSoundMem(std::string("Resources/Sound/SFX_UI_map_close.mp3"));

		distribution = std::discrete_distribution<int32_t>(nodeProbabilities,nodeProbabilities + NodeType::TYPE_NUM);

		rooms_[ NodeType::REINFORCEMENT ] = std::make_unique<ReinforcementNode>();
		rooms_[ NodeType::TRANSACTION ] = std::make_unique<TransactionNode>();
		rooms_[ NodeType::BATTLE ] = std::make_unique<BattleNode>();
		rooms_[ NodeType::SHOP ] = std::make_unique<ShopNode>();
		rooms_[ NodeType::HEALING ] = std::make_unique<HealingNode>();
		rooms_[ NodeType::START ] = std::make_unique<StartNode>();

		bossRoom_ = std::make_unique<BossNode>();
		mapChip_->SetEnemyManager(enemys_);

		for ( auto& room : rooms_ )
		{
			room->SetMapChip(mapChip_);
			room->SetPlayer(player_);
			room->SetNodeManagerr(this);
			room->SetPowerUp(powerUp_);
			room->SetDealer(dealer_);
			room->SetEnemyManager(enemys_);
			room->Initialize();
		}

		bossRoom_->SetMapChip(mapChip_);
		bossRoom_->SetPlayer(player_);
		bossRoom_->SetEnemyManager(enemys_);
		bossRoom_->SetNodeManagerr(this);
		bossRoom_->Initialize();

		bossNode_.type.value = NodeType::BOSS;

		GenerateInitialGrid();

		std::vector<int32_t> startingPoints = GetRandomStartingPoints();

		for ( int32_t k = 0; k < PATHS; k++ )
		{
			int32_t j = startingPoints[ DxLib::GetRand(startingPoints.size() - 1) ];

			int32_t current_j = j;
			for ( int32_t i = 0; i < FLOORS - 1; ++i )
			{
				current_j = SetupConnection(i,current_j);
			}
		}

		for ( int32_t point : startingPoints )
		{
			if ( nodes_[ 0 ][ point ].type.value != NodeType::Type::NO_CHILDREN )
			{
				startNodes_.push_back(&nodes_[ 0 ][ point ]);
				nodes_[ 0 ][ point ].type.value = NodeType::START;
			}
		}

		for ( int32_t i = 0; i < MAP_WIDTH; ++i )
		{
			if ( !nodes_[ FLOORS - 1 ][ i ].previews.empty() )
			{
				nodes_[ FLOORS - 1 ][ i ].nexts.push_back(&bossNode_);
				nodes_[ FLOORS - 1 ][ i ].type.value = NodeType::Type::NONE;
			}
		}

		SetupRoomTypes();

		for ( int32_t i = 0; i < FLOORS; ++i )
		{
			for ( int32_t j = 0; j < MAP_WIDTH; ++j )
			{
				if ( !nodes_[ i ][ j ].nexts.empty() )
				{
					std::vector<Node*>& nodes = nodes_[ i ][ j ].nexts;
					std::sort(nodes.begin(),nodes.end(),[ ] (Node* node,Node* node2)
	 {
			 return node->column < node2->column;
	 });
				}
			}
		}

		std::sort(startNodes_.begin(),startNodes_.end());

		playerNodePos = 0;
		leftBottomX = 450;
		leftBottomY = 650;

		int32_t bossNodeX = 0;
		int32_t nodeCount = 0;

		for ( int32_t i = 0; i < MAP_WIDTH; i++ )
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

		bossNode_.position.y -= Y_DIST*2;
		bossNode_.position.x = bossNodeX / nodeCount;
		bossNode_.row = FLOORS;

		node_ = nullptr;

		circleMouseShape_.SetRadius(2);
		circleShape_.SetRadius(32);
		selectNode_ = startNodes_[ 0 ];
	}
	else
	{
		Reset();
	}

}

void NodeManager::Update()
{
	isNodeReset_ = false;

	if ( isNodeDraw )
	{
		int32_t scroll = GetMouseWheelRotVol();
		if ( scroll == -1 && leftBottomY > STARTNODE_DREW_MAX_Y )
		{
			leftBottomY += scrollSpeed_ * scroll;
		}

		if ( scroll == 1 && nodes_[ playerNodePos - 1 ][ 0 ].position.y + leftBottomY < NODE_DREW_MIN_Y )
		{
			leftBottomY += scrollSpeed_ * scroll;
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
		isNodeReset_ = true;
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_M)||Input::Instance()->TriggerPadKey(PAD_INPUT_8) )
	{
		if ( isNodeDraw )
		{
			PlaySoundMem(closeSound_,DX_PLAYTYPE_BACK);
			isNodeDraw = false;
			
		}
		else
		{
			PlaySoundMem(openSound_,DX_PLAYTYPE_BACK);
			isNodeDraw = true;
		}
	}

	if ( !isNodeDraw )
	{
		node_->Update();
	}

}

void NodeManager::Draw()
{
	node_->Draw();
}

void NodeManager::Reset()
{
	startNodes_.clear();

	for ( int32_t i = 0; i < FLOORS; ++i )
	{
		for ( int32_t j = 0; j < MAP_WIDTH; ++j )
		{
			nodes_[ i ][ j ].nexts.clear();
			nodes_[ i ][ j ].previews.clear();
			nodes_[ i ][ j ].nextDoorsNum = 0;
		}

		nodes_[ i ].clear();
	}

	nodes_.clear();

	GenerateInitialGrid();

	std::vector<int32_t> startingPoints = GetRandomStartingPoints();

	for ( int32_t k = 0; k < PATHS; k++ )
	{
		int32_t j = startingPoints[ DxLib::GetRand(startingPoints.size() - 1) ];

		int32_t current_j = j;
		for ( int32_t i = 0; i < FLOORS - 1; ++i )
		{
			current_j = SetupConnection(i,current_j);
		}
	}

	for ( int32_t point : startingPoints )
	{
		if ( nodes_[ 0 ][ point ].type.value != NodeType::Type::NO_CHILDREN )
		{
			startNodes_.push_back(&nodes_[ 0 ][ point ]);
			nodes_[ 0 ][ point ].type.value = NodeType::START;
		}
	}

	for ( int32_t i = 0; i < MAP_WIDTH; ++i )
	{
		if ( !nodes_[ FLOORS - 1 ][ i ].previews.empty() )
		{
			nodes_[ FLOORS - 1 ][ i ].nexts.push_back(&bossNode_);
			nodes_[ FLOORS - 1 ][ i ].type.value = NodeType::Type::NONE;
		}
	}

	SetupRoomTypes();

	for ( int32_t i = 0; i < FLOORS; ++i )
	{
		for ( int32_t j = 0; j < MAP_WIDTH; ++j )
		{
			if ( !nodes_[ i ][ j ].nexts.empty() )
			{
				std::vector<Node*>& nodes = nodes_[ i ][ j ].nexts;
				std::sort(nodes.begin(),nodes.end(),[ ] (Node* node,Node* node2)
 {
	return node->column < node2->column;
 });
			}
		}
	}

	std::sort(startNodes_.begin(),startNodes_.end());

	playerNodePos = 0;
	leftBottomX = 450;
	leftBottomY = 650;

	int32_t bossNodeX = 0;
	int32_t nodeCount = 0;

	for ( int32_t i = 0; i < MAP_WIDTH; i++ )
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

	bossNode_.position.y -= Y_DIST * 2;
	bossNode_.position.x = bossNodeX / nodeCount;
	bossNode_.row = FLOORS;

	isGameEnd_ = false;
	node_ = nullptr;

	mapChip_->SetEnemyManager(enemys_);

	for ( auto& room : rooms_ )
	{
		room->SetMapChip(mapChip_);
		room->SetPlayer(player_);
		room->SetNodeManagerr(this);
		room->SetPowerUp(powerUp_);
		room->SetDealer(dealer_);
		room->SetEnemyManager(enemys_);
	}

	bossRoom_->SetMapChip(mapChip_);
	bossRoom_->SetPlayer(player_);
	bossRoom_->SetEnemyManager(enemys_);
	bossRoom_->SetNodeManagerr(this);

}

void NodeManager::NodeMapDraw()
{

	playerNodePos = selectNode_->row + 3;
	playerNodePos = min(playerNodePos,FLOORS);

	DrawRotaGraph(GameConfig::GetWindowWidth()/2,GameConfig::GetWindowHeight()/2-2,1.0f,0.0,backGroundImg,true);

	DrawCircle(leftBottomX + selectNode_->position.x,leftBottomY + selectNode_->position.y,20,GetColor(255,0,0));
	for ( int32_t i = 0; i < playerNodePos; ++i )
	{
		for ( int32_t j = 0; j < MAP_WIDTH; ++j )
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
		DrawRotaGraph(leftBottomX + node.position.x,leftBottomY + node.position.y,1.0f,0,bossImg,true);
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

void NodeManager::SetPowerUp(PowerUpCave* powerUp)
{
	powerUp_ = powerUp;
}

void NodeManager::SetDealer(DealDaemon* dealer)
{
	dealer_ = dealer;
}

void NodeManager::SetEnemys(EnemyManager* enemys)
{
	enemys_ = enemys;
}

void NodeManager::SetScrollStop(bool* scrollStop)
{
	rooms_[ NodeType::BATTLE ]->SetScrollStop(scrollStop);
}

bool NodeManager::IsMapDraw()
{
	return isNodeDraw;
}

bool NodeManager::GameEnd()
{
	return isGameEnd_;
}

bool NodeManager::IsNodeReset()
{
	return isNodeReset_;
}

void NodeManager::End()
{
	isGameEnd_ = true;
}

void NodeManager::MapDraw()
{
	if ( isNodeDraw )
	{
		NodeMapDraw();
	}
}

bool NodeManager::StartNodeSelect()
{
	playerNodePos = selectNode_->row + 3;
	playerNodePos = min(playerNodePos,FLOORS);

	int32_t mouseX;
	int32_t mouseY;

	GetMousePoint(&mouseX,&mouseY);

	circleMouseShape_.SetCenter({ float( mouseX ) ,float( mouseY ) });

	selectNodeX = -1;
	selectNodeY = -1;

	for ( int32_t i = 0; i < playerNodePos; ++i )
	{
		for ( int32_t j = 0; j < MAP_WIDTH; ++j )
		{
			circleShape_.SetCenter({ leftBottomX + nodes_[ i ][ j ].position.x,leftBottomY + nodes_[ i ][ j ].position.y });

			if ( Collision::Circle2Circle(circleShape_,circleMouseShape_) && nodes_[ i ][ j ].type.value == NodeType::START )
			{
				selectNodeX = j;
				selectNodeY = i;

				if ( GetMouseInput() & MOUSE_INPUT_LEFT )
				{
					isMouseInput_ = true;
				}
			}
		}
	}

	if ( isMouseInput_ )
	{
		for ( auto& itr : startNodes_ )
		{
			if ( itr->column == selectNodeX && itr->row == selectNodeY )
			{
				nextNode_ = itr;

				return true;
			}
		}
	}


	return false;
}

void NodeManager::StartNodeSelectMapDraw()
{
	selectNode_ = startNodes_[ 0 ];
	playerNodePos = selectNode_->row + 3;
	playerNodePos = min(playerNodePos,FLOORS);

	DrawRotaGraph(GameConfig::GetWindowWidth() / 2,GameConfig::GetWindowHeight() / 2 - 2,1.0f,0.0,backGroundImg,true);

	for ( int32_t i = 0; i < playerNodePos; ++i )
	{
		for ( int32_t j = 0; j < MAP_WIDTH; ++j )
		{
			if ( selectNodeX == j && selectNodeY == i )
			{
				DrawCircle(leftBottomX + nodes_[ i ][ j ].position.x,leftBottomY + nodes_[ i ][ j ].position.y,20,GetColor(255,0,0));
			}

			NodeDrew(leftBottomX,leftBottomY,nodes_[ i ][ j ],true);

			if ( playerNodePos == FLOORS )
			{
				NodeDrew(leftBottomX,leftBottomY,bossNode_,true);
			}
		}
	}
}

void NodeManager::GenerateInitialGrid()
{
	for ( int32_t i = 0; i < FLOORS; ++i )
	{
		std::vector<Node> adjacentRooms;

		for ( int32_t j = 0; j < MAP_WIDTH; ++j )
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

std::vector<int32_t> NodeManager::GetRandomStartingPoints()
{
	std::vector<int32_t> yCoordinates;
	int32_t uniquePoints = 0;

	while ( uniquePoints < 2 )
	{
		uniquePoints = 0;
		yCoordinates.clear();

		for ( int32_t i = 0; i < START_POINT; ++i )
		{
			int32_t startingPoint = DxLib::GetRand(MAP_WIDTH - 1);

			if ( find(yCoordinates.begin(),yCoordinates.end(),startingPoint) == yCoordinates.end() )
			{
				uniquePoints++;
			}

			yCoordinates.push_back(startingPoint);
		}
	}

	return yCoordinates;
}

int32_t NodeManager::SetupConnection(int32_t i,int32_t j)
{
	Node* nextRoom = nullptr;
	Node* currentRoom = &nodes_[ i ][ j ];
	int32_t rand_ = 0;
	int32_t selectJ;
	int32_t random_j = 0;

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

bool NodeManager::WouldCrossExistingPath(int32_t i,int32_t j,Node* room)
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