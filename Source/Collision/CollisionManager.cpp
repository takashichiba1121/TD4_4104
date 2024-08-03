#include "CollisionManager.h"
#include"Collision.h"

#include<limits>

void CollisionManager::AddObject(IObject* object)
{
	objects_.push_front(object);

	if ( object->mapChipObject_ )
	{
		mapChipObjects_.push_front(object);
	}
}

void CollisionManager::RemoveObject(IObject* object)
{
	objects_.remove(object);

	if ( object->mapChipObject_ )
	{
		mapChipObjects_.remove(object);
	}
}

void CollisionManager::SetMapChip(std::vector<std::vector<uint8_t>>* mapChip)
{
	mapChip_ = mapChip;
}

void CollisionManager::SetScreenPos(const Vector2& screenPos)
{
	screenPos_ = screenPos;
}

void CollisionManager::Update()
{
	auto itrA = objects_.begin();

	for ( ; itrA != objects_.end(); ++itrA )
	{
		IObject* objectA = *itrA;

		objectA->Update();

		if ( !objectA->isCollision_ || !objectA->shape_ )
		{
			continue;
		}

		auto itrB = itrA;
		itrB++;

		for ( ; itrB != objects_.end(); ++itrB )
		{
			IObject* objectA = itrA._Ptr->_Myval;
			IObject* objectB = itrB._Ptr->_Myval;

			objectB->Update();

			if ( !objectB->isCollision_ || !objectB->shape_ )
			{
				continue;
			}

			if ( CheckCollisionPair(objectA,objectB) )
			{
				if ( objectA->GetShapeType() == ShapeType::S_CIRCLE && objectB->GetShapeType() == ShapeType::S_CIRCLE )
				{
					CircleShape* circleA = dynamic_cast< CircleShape* >( objectA->GetShape() );
					CircleShape* circleB = dynamic_cast< CircleShape* >( objectB->GetShape() );

					if ( Collision::Circle2Circle(*circleA,*circleB) )
					{
						objectA->collisionInfo_.object = objectB;
						objectA->collisionInfo_.shape = objectB->GetShape();
						objectA->collisionInfo_.userData = objectB->userData_;

						objectB->collisionInfo_.object = objectA;
						objectB->collisionInfo_.shape = objectA->GetShape();
						objectB->collisionInfo_.userData = objectA->userData_;

						objectA->OnCollision();
						objectB->OnCollision();

					}
				}
				else if ( objectA->GetShapeType() == ShapeType::S_RECT && objectB->GetShapeType() == ShapeType::S_RECT )
				{
					RectShape* rectA = dynamic_cast< RectShape* >( objectA->GetShape() );
					RectShape* rectB = dynamic_cast< RectShape* >( objectB->GetShape() );

					if ( Collision::Rect2Rect(*rectA,*rectB) )
					{
						objectA->collisionInfo_.object = objectB;
						objectA->collisionInfo_.shape = objectB->GetShape();
						objectA->collisionInfo_.userData = objectB->userData_;

						objectB->collisionInfo_.object = objectA;
						objectB->collisionInfo_.shape = objectA->GetShape();
						objectB->collisionInfo_.userData = objectA->userData_;

						objectA->OnCollision();
						objectB->OnCollision();
					}
					else if ( objectA->GetShapeType() == ShapeType::S_RECT && objectB->GetShapeType() == ShapeType::S_CIRCLE )
					{
						RectShape* rect = dynamic_cast< RectShape* >( objectA->GetShape() );
						CircleShape* circle = dynamic_cast< CircleShape* >( objectB->GetShape() );

						if ( Collision::Rect2Circe(*rect,*circle) )
						{
							objectA->collisionInfo_.object = objectB;
							objectA->collisionInfo_.shape = objectB->GetShape();
							objectA->collisionInfo_.userData = objectB->userData_;

							objectB->collisionInfo_.object = objectA;
							objectB->collisionInfo_.shape = objectA->GetShape();
							objectB->collisionInfo_.userData = objectA->userData_;

							objectA->OnCollision();
							objectB->OnCollision();
						}
					}
				}
			}
		}
	}

	for ( auto& itr : mapChipObjects_ )
	{

		if (! itr->mapChipObject_ )
		{
			continue;
		}

		uint8_t dir = 0;

		if ( itr->speed_.y == 0 )
		{

		}
		else if ( itr->speed_.y < 0 )
		{
			if ( TopCollision(itr) )
			{
				dir |= 0b1 << OnDir::UP;
			}
		}
		else
		{
			if ( DownCollision(itr) )
			{
				dir |= 0b1 << OnDir::BOTTOM;
			}
		}

		if ( itr->speed_.x  == 0)
		{

		}
		else if ( itr->speed_.x < 0 )
		{
			if ( LeftCollision(itr) )
			{
				dir |= 0b1 << 0b1 << OnDir::LEFT;
			}
		}
		else
		{
			if ( RightCollision(itr) )
			{
				dir |= 0b1 << OnDir::RIGHT;
			}
		}

		itr->speed_.x = 0;
		itr->speed_.y = 0;
		itr->dir_ = dir;
	}
}

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::CheckCollisionPair(IObject* objectA,IObject* objectB)
{
	if ( !( objectA->GetCollisionAttribute() & objectB->GetCollisionMask() ) ||
		!( objectA->GetCollisionMask() & objectB->GetCollisionAttribute() ) )
	{
		return false;
	}

	return true;
}

bool CollisionManager::DownCollision(IObject* object)
{

	bool on = false;

	float objectX = object->center_->x + 37;
	float objectY = object->center_->y + 32;

	uint32_t downLeftX = static_cast< int32_t >( ( objectX - object->r_.x ) / BLOCK_SIZE );
	uint32_t downRightX = static_cast< int32_t >( ( objectX + object->r_.x + -1 ) / BLOCK_SIZE );

	uint32_t downLeftY = static_cast< int32_t >( ( ( objectY + object->r_.y - 1 ) + object->speed_.y ) / BLOCK_SIZE );
	uint32_t downRightY = static_cast< int32_t >( ( ( objectY + object->r_.y - 1 ) + object->speed_.y ) / BLOCK_SIZE );

	if ( downLeftY >= mapChip_->size() || downRightY >= mapChip_->size() )
	{
		on = true;
	}
	else if ( mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NONE && mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NEXT ||
			  mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NONE && mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NEXT )
	{
		on = true;
	}


	if ( !on)
	{
		object->center_->y += object->speed_.y;

		return false;
	}
	else
	{

		downLeftY = static_cast< int32_t >( ( objectY + object->r_.y - 1 ) / BLOCK_SIZE );
		downRightY = static_cast< int32_t >( ( objectY + object->r_.y - 1 ) / BLOCK_SIZE );

		downLeftY = std::min(downLeftY,mapChip_->size() - 1);
		downRightY = std::min(downLeftY,mapChip_->size() - 1);

		on = false;

		if ( mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NONE && mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NEXT ||
			  mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NONE && mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NEXT )
		{
			on = true;
		}

		if ( !on )
		{
			while ( !on )
			{
				downLeftY = static_cast< int32_t >( ( ( objectY + object->r_.y  - 1 ) + 1 ) / BLOCK_SIZE );
				downRightY = static_cast< int32_t >( ( ( objectY + object->r_.y  - 1 ) + 1 ) / BLOCK_SIZE );
				downLeftY = std::min(downLeftY,mapChip_->size() - 1);
				downRightY = std::min(downLeftY,mapChip_->size() - 1);

				on = false;

				if ( downLeftY == downLeftY,mapChip_->size() - 1 || downRightY == mapChip_->size() - 1 )
				{
					break;
				}

				if ( mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NONE && mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NEXT ||
					 mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NONE && mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NEXT )
				{
					on = true;
				}

				if ( !on )
				{
					object->center_->y += 1.0f;
				}	objectY = object->center_->y + 32;
			}
		}

		return true;
	}
}

bool CollisionManager::TopCollision(IObject* object)
{
	bool on = false;

	float objectX = object->center_->x + 37;
	float objectY = object->center_->y + 32;

	uint32_t topLeftX = static_cast< int32_t >( ( objectX - object->r_.x ) / BLOCK_SIZE );
	uint32_t topRightX = static_cast< int32_t >( ( objectX + object->r_.x - 1 ) / BLOCK_SIZE );

	uint32_t topLeftY = static_cast< int32_t >( ( ( objectY - object->r_.y - 1 ) + object->speed_.y ) / BLOCK_SIZE );
	uint32_t topRightY = static_cast< int32_t >( ( ( objectY - object->r_.y ) + object->speed_.y ) / BLOCK_SIZE );

	if ( topRightY >= mapChip_->size() || topLeftY >= mapChip_->size() )
	{
		on = true;
	}
	else if ( mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NONE && mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NEXT ||
			  mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NONE && mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NEXT )
	{
		on = true;
	}

	if ( !on )
	{
		object->center_->y += object->speed_.y;

		return false;
	}
	else
	{
		topLeftY = static_cast< int32_t >( ( objectY - object->r_.y ) / BLOCK_SIZE );
		topRightY = static_cast< int32_t >( ( objectY - object->r_.y ) / BLOCK_SIZE );
		on = false;

		if ( mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NONE && mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NEXT ||
			 mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NONE && mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NEXT )
		{
			on = true;
		}

		if ( !on )
		{
			while ( !on )
			{
				topLeftY = static_cast< int32_t >( ( ( objectY - object->r_.y - 1 ) ) / BLOCK_SIZE );
				topRightY = static_cast< int32_t >( ( ( objectY - object->r_.y - 1 ) ) / BLOCK_SIZE );
				on = false;

				if ( mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NONE && mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NEXT ||
					 mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NONE && mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NEXT )
				{
					on = true;
				}

				if ( !on )
				{
					object->center_->y -= 1.0f;
					objectY = object->center_->y + 32;
				}

			}
		}

		return true;
	}
}

bool CollisionManager::LeftCollision(IObject* object)
{
	bool on = false;

	float objectX = object->center_->x + 37;
	float objectY = object->center_->y + 32;

	uint32_t topLeftX = static_cast< uint32_t >( ( ( objectX - object->r_.x ) + object->speed_.x ) / BLOCK_SIZE );
	uint32_t downLeftX = static_cast< uint32_t >( ( ( objectX - object->r_.x ) + object->speed_.x ) / BLOCK_SIZE );

	uint32_t topLeftY = static_cast< uint32_t >( ( objectY - object->r_.y ) / BLOCK_SIZE );
	uint32_t downLeftY = static_cast< uint32_t >( ( objectY + object->r_.y - 1 ) / BLOCK_SIZE );

	if ( downLeftX >= mapChip_->data()[ topLeftY ].size() || topLeftX >= mapChip_->data()[ topLeftY ].size() )
	{
		on = true;
	}
	else if ( mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NONE && mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NEXT ||
			  mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NONE && mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NEXT )
	{
		on = true;
	}

	if (!on )
	{
		object->center_->x += object->speed_.x;

		return false;
	}
	else
	{
		topLeftX = static_cast< int32_t >( ( ( objectX - object->r_.x ) ) / BLOCK_SIZE );
		downLeftX = static_cast< int32_t >( ( ( objectX - object->r_.x ) ) / BLOCK_SIZE );
		on = false;

		if ( mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NONE && mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NEXT ||
			 mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NONE && mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NEXT )
		{
			on = true;
		}

		if ( !on )
		{
			while ( !on )
			{
				topLeftX = static_cast< int32_t >( ( ( objectX - object->r_.x ) - 1 ) / BLOCK_SIZE );
				downLeftX = static_cast< int32_t >( ( ( objectX - object->r_.x ) - 1 ) / BLOCK_SIZE );
				on = false;

				if ( mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NONE && mapChip_->data()[ topLeftY ][ topLeftX ] != ChipIndex::NEXT ||
					 mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NONE && mapChip_->data()[ downLeftY ][ downLeftX ] != ChipIndex::NEXT )
				{
					on = true;
				}

				if ( !on )
				{
					object->center_->x -= 1;
					objectX = object->center_->x + 37;
				}

			}
		}

		return true;
	}
}

bool CollisionManager::RightCollision(IObject* object)
{
	bool on = false;

	float objectX = object->center_->x + 37;
	float objectY = object->center_->y + 32;

	uint32_t topRightX = static_cast< int32_t >( ( ( objectX + object->r_.x - 1 ) + object->speed_.x ) / BLOCK_SIZE );
	uint32_t downRightX = static_cast< int32_t >( ( ( objectX + object->r_.x - 1 ) + object->speed_.x ) / BLOCK_SIZE );

	uint32_t topRightY = static_cast< int32_t >( ( objectY - object->r_.y ) / BLOCK_SIZE );
	uint32_t downRightY = static_cast< int32_t >( ( objectY + object->r_.y - 1 ) / BLOCK_SIZE );

	if ( topRightX >= mapChip_->data()[ topRightY ].size() || downRightX >= mapChip_->data()[ topRightY ].size() )
	{
		on = true;
	}
	else if ( mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NONE && mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NEXT ||
			  mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NONE && mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NEXT )
	{
		on = true;
	}

	if (! on )
	{
		object->center_->x += object->speed_.x;

		return false;
	}
	else
	{
		topRightX = static_cast< int32_t >( ( ( objectX + object->r_.x - 1 ) ) / BLOCK_SIZE );
		downRightX = static_cast< int32_t >( ( ( objectX + object->r_.x - 1 ) ) / BLOCK_SIZE );
		on = false;

		if ( mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NONE && mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NEXT ||
			 mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NONE && mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NEXT )
		{
			on = true;
		}

		if (! on )
		{
			while ( !on )
			{
				topRightX = static_cast< int32_t >( ( ( objectX + object->r_.x - 1 ) + 1 ) / BLOCK_SIZE );
				downRightX = static_cast< int32_t >( ( ( objectX + object->r_.x - 1 ) + 1 ) / BLOCK_SIZE );
				on = false;

				if ( mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NONE && mapChip_->data()[ topRightY ][ topRightX ] != ChipIndex::NEXT ||
					mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NONE && mapChip_->data()[ downRightY ][ downRightX ] != ChipIndex::NEXT )
				{
					on = true;
				}

				if ( !on )
				{
					object->center_->x += 1;
					objectX = object->center_->x + 37;
				}
			}
		}
		return true;
	}
}
