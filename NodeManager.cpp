#include "NodeManager.h"
#include"Vector2.h"
#include<algorithm>
#include<DxlibInclude.h>

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
	distribution = std::discrete_distribution<int>(probabilities,probabilities + BaseNode::Type::TYPE_NUM);

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

	SetupRoomTypes();

	int b = 0;
	b++;
}

void NodeManager::Update()
{
	if ( nextNode_ )
	{
		nextNode_->previews_ = node_;
		node_ = nextNode_;

		node_->Initialize();

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
			nodes_[ i ][ j ]->nexts_.clear();
		}
	}

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
}

void NodeManager::ChangeNodeSelectLeft()
{
	nextNode_ = node_->nexts_[ 0 ];
}

void NodeManager::ChangeNodeSelectCenter()
{
	nextNode_ = node_->nexts_[ 1 ];
}

void NodeManager::ChangeNodeSelectRight()
{
	nextNode_ = node_->nexts_[ 2 ];
}

void NodeManager::NodeDrew()
{
	for ( int i = 0; i < FLOORS; ++i )
	{
		for ( int j = 0; j < MAP_WIDTH; ++j )
		{
			if ( !nodes_[ i ][ j ]->nexts_.empty() )
			{
				DrawCircle(100 + j * 30,( 60 + FLOORS * 30 ) - ( 60 + i * 30 ),3,GetColor(255,255,255));

				for ( size_t k = 0; k < nodes_[ i ][ j ]->nexts_.size(); k++ )
				{
					DrawLine(100 + j * 30,( 60 + FLOORS * 30 ) - ( 60 + i * 30 ),100 + nodes_[ i ][ j ]->nexts_[ k ]->column_ * 30,( 60 + FLOORS * 30 ) - ( 60 + nodes_[ i ][ j ]->nexts_[ k ]->row_ * 30 ),GetColor(255,255,255));
				}
			}
		}
	}
}

void NodeManager::GenerateInitialGrid()
{
	for ( int i = 0; i < FLOORS; ++i )
	{
		std::vector<BaseNode*> adjacentRooms;

		for ( int j = 0; j < MAP_WIDTH; ++j )
		{
			BaseNode* currentRoom = new BaseNode();
			Vector2 offset(GetRand(0,PLACEMENT_RANDOMNESS),GetRand(0,PLACEMENT_RANDOMNESS));
			currentRoom->position_ = Vector2(j * X_DIST,i * -Y_DIST) + offset;
			currentRoom->row_ = i;
			currentRoom->column_ = j;
			currentRoom->type_ = BaseNode::Type::NO_CHILDREN;

			// Boss room has a non-random Y
			if ( i == FLOORS - 1 )
			{
				currentRoom->position_.y = i * -Y_DIST;
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
	BaseNode* nextRoom = nullptr;
	BaseNode* currentRoom = nodes_[ i ][ j ];
	int rand_ = 0;
	int selectJ;
	int random_j =0;

	{
		oldRandomJ = random_j;
		oldRand = rand_;
		while ( random_j == oldRandomJ )
		{
			rand_ = GetRand(-1,1);
			selectJ = rand_ + j;
			random_j = std::clamp(selectJ,0,MAP_WIDTH - 1);
		}

		nextRoom = nodes_[ i + 1 ][ random_j ];

		if ( WouldCrossExistingPath(i,j,nextRoom) )
		{
			rand_ *= -1;
			selectJ = rand_ + j;
			random_j = std::clamp(selectJ,0,MAP_WIDTH - 1);
			nextRoom = nodes_[ i + 1 ][ random_j ];

			if ( WouldCrossExistingPath(i,j,nextRoom) )
			{
				oldRandomJ = random_j;
				oldRand = rand_;
				while ( random_j == oldRandomJ  )
				{
					rand_ = GetRand(-1,1);
					selectJ = rand_ + j;
					random_j = std::clamp(selectJ,0,MAP_WIDTH - 1);
				}

				nextRoom = nodes_[ i + 1 ][ random_j ];
			}
		}
	}

	currentRoom->nexts_.push_back(nextRoom);
	currentRoom->NONE;

	return nextRoom->column_;
}

bool NodeManager::WouldCrossExistingPath(int i,int j,BaseNode* room)
{
	BaseNode* leftNeighbour = nullptr;
	BaseNode* rightNeighbour = nullptr;

	if ( j > 0 )
	{
		leftNeighbour = nodes_[ i ][ j - 1 ];
	}
	if ( j < MAP_WIDTH - 1 )
	{
		rightNeighbour = nodes_[ i ][ j + 1 ];
	}

	if ( rightNeighbour && room->column_ > j )
	{
		for ( BaseNode* nextRoom : rightNeighbour->nexts_ )
		{
			if ( nextRoom->column_ < room->column_ )
			{
				return true;
			}
		}
	}

	if ( leftNeighbour && room->column_ < j )
	{
		for ( BaseNode* next_room : leftNeighbour->nexts_ )
		{
			if ( next_room->column_ > room->column_ )
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
		for ( BaseNode* room : current_floor )
		{
			for ( BaseNode* next_room : room->nexts_ )
			{
				if ( next_room->type_ == BaseNode::NONE )
				{
					SetRoomRandomly(next_room);
				}
			}
		}
	}
}

bool NodeManager::RoomHasParentOfType(BaseNode* room,BaseNode::Type type)
{
	std::vector<BaseNode*> parents;
	if ( room->column_ > 0 && room->row_ > 0 )
	{
		BaseNode* parentCandidate = nodes_[ room->row_ - 1 ][ room->column_ - 1 ];
		if ( find(parentCandidate->nexts_.begin(),parentCandidate->nexts_.end(),room) != parentCandidate->nexts_.end() )
		{
			parents.push_back(parentCandidate);
		}
	}
	if ( room->row_ > 0 )
	{
		BaseNode* parentCandidate = nodes_[ room->row_ - 1 ][ room->column_ ];
		if ( find(parentCandidate->nexts_.begin(),parentCandidate->nexts_.end(),room) != parentCandidate->nexts_.end() )
		{
			parents.push_back(parentCandidate);
		}
	}
	if ( room->column_ < MAP_WIDTH - 1 && room->row_ > 0 )
	{
		BaseNode* parentCandidate = nodes_[ room->row_ - 1 ][ room->column_ + 1 ];
		if ( find(parentCandidate->nexts_.begin(),parentCandidate->nexts_.end(),room) != parentCandidate->nexts_.end() )
		{
			parents.push_back(parentCandidate);
		}
	}

	for ( BaseNode* parent : parents )
	{
		if ( parent->type_ == type )
		{
			return true;
		}
	}

	return false;
}

void NodeManager::SetRoomRandomly(BaseNode* roomToSet)
{
	BaseNode::Type typeCandidate;
	std::mt19937 gen(std::time(nullptr));
	typeCandidate = static_cast< BaseNode::Type >( distribution(gen) );
	roomToSet->type_ = typeCandidate;
}