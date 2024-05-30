#include "CollisionManager.h"
#include"Collision.h"


void CollisionManager::AddObject(IObject* object)
{
	objects_.push_front(object);
}

void CollisionManager::RemoveObject(IObject* object)
{
	objects_.remove(object);
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
				if ( objectA->GetShapeType() == ShapeType::CIRCLE && objectB->GetShapeType() == ShapeType::CIRCLE )
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
				else if ( objectA->GetShapeType() == ShapeType::RECT && objectB->GetShapeType() == ShapeType::RECT )
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
