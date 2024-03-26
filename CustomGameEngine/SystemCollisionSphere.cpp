#include "SystemCollisionSphere.h"
namespace Engine {
	SystemCollisionSphere::SystemCollisionSphere(EntityManager* entityManager, CollisionManager* collisionManager) : SystemCollision(entityManager, collisionManager)
	{
	}

	SystemCollisionSphere::~SystemCollisionSphere()
	{

	}

	void SystemCollisionSphere::OnAction(Entity* entity)
	{
		if ((entity->Mask() & MASK) == MASK) {
			std::vector<Component*> components = entity->Components();

			ComponentTransform* transform = nullptr;
			for (Component* c : components) {
				transform = dynamic_cast<ComponentTransform*>(c);
				if (transform != nullptr) {
					break;
				}
			}

			ComponentCollisionSphere* collider = nullptr;
			for (Component* c : components) {
				collider = dynamic_cast<ComponentCollisionSphere*>(c);
				if (collider != nullptr) {
					break;
				}
			}

			// Loop through all over entities to find another AABB collider entity
			for (Entity* e : entityManager->Entities()) {
				if ((e->Mask() & MASK) == MASK) {
					// Check if this entity has already checked for collisions with current entity in a previous run during this frame
					if (!collider->HasEntityAlreadyBeenChecked(e) && e != entity) {

						std::vector<Component*> components2 = e->Components();

						ComponentTransform* transform2 = nullptr;
						for (Component* c : components2) {
							transform2 = dynamic_cast<ComponentTransform*>(c);
							if (transform2 != nullptr) {
								break;
							}
						}

						ComponentCollisionSphere* collider2 = nullptr;
						for (Component* c : components2) {
							collider2 = dynamic_cast<ComponentCollisionSphere*>(c);
							if (collider2 != nullptr) {
								break;
							}
						}

						// Check for collision
						Collision(transform, collider, transform2, collider2);
					}
				}
			}
		}
	}

	void SystemCollisionSphere::AfterAction()
	{
		// Loop through all collision entities and clear EntitiesCheckedThisFrame
		for (Entity* e : entityManager->Entities()) {
			if ((e->Mask() & MASK) == MASK) {
				dynamic_cast<ComponentCollisionSphere*>(e->GetComponent(COMPONENT_COLLISION_SPHERE))->ClearEntitiesCheckedThisFrame();
			}
		}
	}

	CollisionData SystemCollisionSphere::Intersect(ComponentTransform* transform, ComponentCollision* collider, ComponentTransform* transform2, ComponentCollision* collider2)
	{
		float scaledRadius1 = dynamic_cast<ComponentCollisionSphere*>(collider)->CollisionRadius() * transform->GetBiggestScaleFactor();
		float scaledRadius2 = dynamic_cast<ComponentCollisionSphere*>(collider2)->CollisionRadius() * transform2->GetBiggestScaleFactor();

		float distance = glm::distance(transform->GetWorldPosition(), transform2->GetWorldPosition());

		float combinedRadius = scaledRadius1 + scaledRadius2;
		
		CollisionData collision;
		if (distance < combinedRadius) {
			collision.isColliding = true;
			collision.collisionPenetration = combinedRadius - distance;
			collision.collisionNormal = -glm::normalize(transform2->GetWorldPosition() - transform->GetWorldPosition());
			collision.localCollisionPoint = collision.collisionNormal * dynamic_cast<ComponentCollisionSphere*>(collider)->CollisionRadius();
			collision.otherLocalCollisionPoint = -collision.collisionNormal * dynamic_cast<ComponentCollisionSphere*>(collider2)->CollisionRadius();
			collision.collidingObject = transform->GetOwner();
			collision.otherCollidingObject = transform2->GetOwner();
		}
		else {
			collision.isColliding = false;
		}

		return collision;
	}
}