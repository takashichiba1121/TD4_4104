#include "NodeManager.h"
#include"Vector2.h"
#include<algorithm>

NodeManager* NodeManager::GetInstance()
{
	static NodeManager instance;
    return &instance;
}

void NodeManager::Initialize()
{
	GenerateInitialGrid();

	std::vector<int> startingPoints = GetRandomStartingPoints();

	for (int j : startingPoints)
	{
		int current_j = j;
		for (int i = 0; i < FLOORS - 1; ++i)
		{
			current_j = SetupConnection(i, current_j);
		}
	}
}

void NodeManager::Update()
{
	if ( nextNode_)
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

void NodeManager::GenerateInitialGrid()
{
	for (int i = 0; i < FLOORS; ++i)
	{
		std::vector<BaseNode*> adjacentRooms;

		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			BaseNode* currentRoom = new BaseNode();
			Vector2 offset(rand() % PLACEMENT_RANDOMNESS, rand() % PLACEMENT_RANDOMNESS);
			currentRoom->position_ = Vector2(j * X_DIST, i * -Y_DIST) + offset;
			currentRoom->row_ = i;
			currentRoom->column_ = j;

			// Boss room has a non-random Y
			if (i == FLOORS - 1)
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

	while (uniquePoints < 2)
	{
		uniquePoints = 0;
		yCoordinates.clear();

		for (int i = 0; i < PATHS; ++i)
		{
			int startingPoint = rand() % MAP_WIDTH;

			if (find(yCoordinates.begin(), yCoordinates.end(), startingPoint) == yCoordinates.end())
			{
				uniquePoints++;
			}

			yCoordinates.push_back(startingPoint);
		}
	}

	return yCoordinates;
}

int NodeManager::SetupConnection(int i, int j)
{
	BaseNode* nextRoom = nullptr;
	BaseNode* currentRoom = nodes_[i][j];

	while (!nextRoom || WouldCrossExistingPath(i, j, nextRoom))
	{
		int random_j = std::clamp(rand() % (j + 2) - 1, 0, MAP_WIDTH - 1);
		nextRoom = nodes_[i + 1][random_j];
	}

	currentRoom->nexts_.push_back(nextRoom);

	return nextRoom->column_;
}

bool NodeManager::WouldCrossExistingPath(int i, int j, BaseNode* room)
{
	BaseNode* leftNeighbour = nullptr;
	BaseNode* rightNeighbour = nullptr;

	if (j > 0)
	{
		leftNeighbour = nodes_[i][j - 1];
	}
	if (j < MAP_WIDTH - 1)
	{
		rightNeighbour = nodes_[i][j + 1];
	}

	if (rightNeighbour && room->column_ > j)
	{
		for (BaseNode* nextRoom : rightNeighbour->nexts_)
		{
			if (nextRoom->column_ < room->column_)
			{
				return true;
			}
		}
	}

	if (leftNeighbour && room->column_ < j)
	{
		for (BaseNode* next_room : leftNeighbour->nexts_)
		{
			if (next_room->column_ > room->column_)
			{
				return true;
			}
		}
	}

	return false;
}

void NodeManager::SetupBossRoom()
{
	int middle = floor(MAP_WIDTH * 0.5);
	BaseNode* bossRoom = nodes_[FLOORS - 1][middle];

	for (int j = 0; j < MAP_WIDTH; ++j)
	{
		BaseNode* currentRoom = nodes_[FLOORS - 2][j];
		if (!currentRoom->nexts_.empty())
		{
			currentRoom->nexts_.clear();
			currentRoom->nexts_.push_back(bossRoom);
		}
	}

	bossRoom->type_ = BaseNode::BOSS;
}

void NodeManager::SetupRandomRoomWeights()
{
	randomRoomTypeWeights[BaseNode::MONSTER] = MONSTER_ROOM_WEIGHT;
	randomRoomTypeWeights[BaseNode::CAMPFIRE] = MONSTER_ROOM_WEIGHT + CAMPFIRE_ROOM_WEIGHT;
	randomRoomTypeWeights[BaseNode::SHOP] = MONSTER_ROOM_WEIGHT + CAMPFIRE_ROOM_WEIGHT + SHOP_ROOM_WEIGHT;

	randomRoomTypeWeights = randomRoomTypeWeights[BaseNode::SHOP];
}

void NodeManager::SetupRoomTypes()
{
	for (BaseNode* room : nodes_[0])
	{
		if (!room->nexts_.empty())
		{
			room->type_ = BaseNode::MONSTER;
		}
	}

	for (BaseNode* room : nodes_[8])
	{
		if (!room->next_rooms.empty())
		{
			room->type = Room::TREASURE;
		}
	}

	for (BaseNode* room : nodes_[13])
	{
		if (!room->next_rooms.empty())
		{
			room->type = Room::CAMPFIRE;
		}
	}

	for (auto& current_floor : nodes_)
	{
		for (BaseNode* room : current_floor)
		{
			for (BaseNode* next_room : room->next_rooms)
			{
				if (next_room->type == Room::NOT_ASSIGNED)
				{
					_set_room_randomly(next_room);
				}
			}
		}
	}
}
