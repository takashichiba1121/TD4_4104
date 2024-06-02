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

void CollisionManager::SetMapChip(const std::vector<std::vector<uint8_t>>& mapChip)
{
	mapChip_ = mapChip;
}

void CollisionManager::Update()
{
	auto itrA = objects_.begin();

	for ( ; itrA != objects_.end(); ++itrA )
	{
		auto itrB = itrA;
		itrB++;

		for ( ; itrB != objects_.end(); ++itrB )
		{
			IObject* objectA = *itrA;
			IObject* objectB = *itrB;

			objectA->Update();
			objectB->Update();

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
				}
			}
		}
	}

	for ( auto itr : mapChipObjects_ )
	{
		itr->dir_ = 0;

		if ( itr->speed_.x < 0 )
		{
			if ( LeftCollision(itr) )
			{
				itr->dir_ |= 0b1 << 0b1 << OnDir::LEFT;
			}
		}
		else
		{
			if ( RightCollision(itr) )
			{
				itr->dir_ |= 0b1 << OnDir::RIGHT;
			}
		}

		if ( itr->speed_.y < 0 )
		{
			if ( TopCollision(itr) )
			{
				itr->dir_ |= 0b1 << OnDir::UP;
			}
		}
		else
		{
			if ( DownCollision(itr) )
			{
				itr->dir_ |= 0b1 << OnDir::BOTTOM;
			}
		}

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
	int32_t downLeftX = static_cast< int32_t >( ( object->center_->x - object->r_.x ) / BLOCK_SIZE );
	int32_t downRightX = static_cast< int32_t >( ( object->center_->x + object->r_.x - 1 ) / BLOCK_SIZE );

	int32_t downLeftY = static_cast< int32_t >( ( ( object->center_->y + object->r_.y - 1 ) + object->speed_.y ) / BLOCK_SIZE );
	int32_t downRightY = static_cast< int32_t >( ( ( object->center_->y + object->r_.y - 1 ) + object->speed_.y ) / BLOCK_SIZE );

	if ( mapChip_[ downLeftY ][ downLeftX ] == ChipIndex::NONE &&
		 mapChip_[ downRightY ][ downRightX ] == ChipIndex::NONE )
	{
		object->center_->y += object->speed_.y;

		return false;
	}
	else
	{

		downLeftY = static_cast< int32_t >( ( object->center_->y + object->r_.y - 1 ) / BLOCK_SIZE );
		downRightY = static_cast< int32_t >( ( object->center_->y + object->r_.y - 1 ) / BLOCK_SIZE );

		if ( mapChip_[ downLeftY ][ downLeftX ] == ChipIndex::NONE &&
			 mapChip_[ downRightY ][ downRightX ] == ChipIndex::NONE )
		{
			while ( 1 )
			{
				downLeftY = static_cast< int32_t >( ( ( object->center_->y + object->r_.y - 1 ) + 1 ) / BLOCK_SIZE );
				downRightY = static_cast< int32_t >( ( ( object->center_->y + object->r_.y - 1 ) + 1 ) / BLOCK_SIZE );


				if ( mapChip_[ downLeftY ][ downLeftX ] == ChipIndex::NONE &&
					 mapChip_[ downRightY ][ downRightX ] == ChipIndex::NONE )
				{
					object->center_->y += 1.0f;
				}
				else
				{
					break;
				}

			}
		}

		return true;
	}
}

bool CollisionManager::TopCollision(IObject* object)
{

	int32_t topLeftX = static_cast< int32_t >( ( object->center_->x - object->r_.x ) / BLOCK_SIZE );
	int32_t topRightX = static_cast< int32_t >( ( object->center_->x + object->r_.x - 1 ) / BLOCK_SIZE );

	int32_t topLeftY = static_cast< int32_t >( ( ( object->center_->y - object->r_.y - 1 ) + object->speed_.y ) / BLOCK_SIZE );
	int32_t topRightY = static_cast< int32_t >( ( ( object->center_->y - object->r_.y ) + object->speed_.y ) / BLOCK_SIZE );

	if ( mapChip_[ topLeftY ][ topLeftX ] == ChipIndex::NONE &&
		mapChip_[ topRightY ][ topRightX ] == ChipIndex::NONE )
	{
		object->center_->y += object->speed_.y;

		return false;
	}
	else
	{
		topLeftY = static_cast< int32_t >( ( object->center_->y - object->r_.y ) / BLOCK_SIZE );
		topRightY = static_cast< int32_t >( ( object->center_->y - object->r_.y ) / BLOCK_SIZE );

		if ( mapChip_[ topLeftY ][ topLeftX ] == ChipIndex::NONE &&
			 mapChip_[ topRightY ][ topRightX ] == ChipIndex::NONE )
		{
			while ( 1 )
			{
				topLeftY = static_cast< int32_t >( ( ( object->center_->y - object->r_.y - 1 ) ) / BLOCK_SIZE );
				topRightY = static_cast< int32_t >( ( ( object->center_->y - object->r_.y - 1 ) ) / BLOCK_SIZE );


				if ( mapChip_[ topLeftY ][ topLeftX ] == ChipIndex::NONE &&
					 mapChip_[ topRightY ][ topRightX ] == ChipIndex::NONE )
				{
					object->center_->y -= 1.0f;
				}
				else
				{
					break;
				}

			}
		}

		return true;
	}
}

bool CollisionManager::LeftCollision(IObject* object)
{
	int32_t topLeftX = static_cast< int32_t >( ( ( object->center_->x - object->r_.x ) + object->speed_.x ) / BLOCK_SIZE );
	int32_t downLeftX = static_cast< int32_t >( ( ( object->center_->x - object->r_.x ) + object->speed_.x ) / BLOCK_SIZE );

	int32_t topLeftY = static_cast< int32_t >( ( object->center_->y - object->r_.y ) / BLOCK_SIZE );
	int32_t downLeftY = static_cast< int32_t >( ( object->center_->y + object->r_.y - 1 ) / BLOCK_SIZE );

	if ( mapChip_[ topLeftY ][ topLeftX ] == ChipIndex::NONE &&
		 mapChip_[ downLeftY ][ downLeftX ] == ChipIndex::NONE )
	{
		object->center_->x += object->speed_.x;

		return false;
	}
	else
	{
		topLeftX = static_cast< int32_t >( ( ( object->center_->x - object->r_.x ) ) / BLOCK_SIZE );
		downLeftX = static_cast< int32_t >( ( ( object->center_->x - object->r_.x ) ) / BLOCK_SIZE );

		if ( mapChip_[ topLeftY ][ topLeftX ] == ChipIndex::NONE &&
			 mapChip_[ downLeftY ][ downLeftX ] == ChipIndex::NONE )
		{
			while ( 1 )
			{
				topLeftX = static_cast< int32_t >( ( ( object->center_->x - object->r_.x ) - 1 ) / BLOCK_SIZE );
				downLeftX = static_cast< int32_t >( ( ( object->center_->x - object->r_.x ) - 1 ) / BLOCK_SIZE );


				if ( mapChip_[ topLeftY ][ topLeftX ] == ChipIndex::NONE &&
					 mapChip_[ downLeftY ][ downLeftX ] == ChipIndex::NONE )
				{
					object->center_->x -= 1;
				}
				else
				{
					break;
				}

			}
		}

		return true;
	}
}

bool CollisionManager::RightCollision(IObject* object)
{
	int32_t topRightX = static_cast< int32_t >( ( ( object->center_->x + object->r_.x - 1 ) + object->speed_.x ) / BLOCK_SIZE );
	int32_t downRightX = static_cast< int32_t >( ( ( object->center_->x + object->r_.x - 1 ) + object->speed_.x ) / BLOCK_SIZE );

	int32_t topRightY = static_cast< int32_t >( ( object->center_->y - object->r_.y ) / BLOCK_SIZE );
	int32_t downRightY = static_cast< int32_t >( ( object->center_->y + object->r_.y - 1 ) / BLOCK_SIZE );

	if ( mapChip_[ topRightY ][ topRightX ] == ChipIndex::NONE &&
		 mapChip_[ downRightY ][ downRightX ] == ChipIndex::NONE )
	{
		object->center_->x += object->speed_.x;

		return false;
	}
	else
	{
		topRightX = static_cast< int32_t >( ( ( object->center_->x + object->r_.x - 1 ) ) / BLOCK_SIZE );
		downRightX = static_cast< int32_t >( ( ( object->center_->x + object->r_.x - 1 ) ) / BLOCK_SIZE );

		if ( mapChip_[ topRightY ][ topRightX ] == ChipIndex::NONE &&
			 mapChip_[ downRightY ][ downRightX ] == ChipIndex::NONE )
		{
			while ( 1 )
			{
				topRightX = static_cast< int32_t >( ( ( object->center_->x + object->r_.x - 1 ) + 1 ) / BLOCK_SIZE );
				downRightX = static_cast< int32_t >( ( ( object->center_->x + object->r_.x - 1 ) + 1 ) / BLOCK_SIZE );


				if ( mapChip_[ topRightY ][ topRightX ] == ChipIndex::NONE &&
					 mapChip_[ downRightY ][ downRightX ] == ChipIndex::NONE )
				{
					object->center_->x += 1;
				}
				else
				{
					break;
				}

			}
		}

		return true;
	}
}
