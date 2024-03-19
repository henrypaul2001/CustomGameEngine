#include "SystemCollisionSphereAABB.h"
namespace Engine {
	SystemCollisionSphereAABB::SystemCollisionSphereAABB(EntityManager* entityManager) : SystemCollision(entityManager)
	{

	}

	SystemCollisionSphereAABB::~SystemCollisionSphereAABB()
	{

	}

	void SystemCollisionSphereAABB::OnAction(Entity* entity)
	{
		if ((entity->Mask() & SPHERE_MASK) == SPHERE_MASK) {
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
				if ((e->Mask() & AABB_MASK) == AABB_MASK) {
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

						ComponentCollisionAABB* collider2 = nullptr;
						for (Component* c : components2) {
							collider2 = dynamic_cast<ComponentCollisionAABB*>(c);
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

	void SystemCollisionSphereAABB::AfterAction()
	{
		// Loop through all collision entities and clear EntitiesCheckedThisFrame
		for (Entity* e : entityManager->Entities()) {
			if ((e->Mask() & SPHERE_MASK) == SPHERE_MASK) {
				dynamic_cast<ComponentCollisionSphere*>(e->GetComponent(COMPONENT_COLLISION_SPHERE))->ClearEntitiesCheckedThisFrame();
			}
			else if ((e->Mask() & AABB_MASK) == AABB_MASK) {
				dynamic_cast<ComponentCollisionAABB*>(e->GetComponent(COMPONENT_COLLISION_AABB))->ClearEntitiesCheckedThisFrame();
			}
		}
	}

	bool SystemCollisionSphereAABB::Intersect(ComponentTransform* transform, ComponentCollision* collider, ComponentTransform* transform2, ComponentCollision* collider2)
	{
		// collider = sphere
		// collider2 = AABB
		AABBPoints worldSpaceBounds = dynamic_cast<ComponentCollisionAABB*>(collider2)->GetWorldSpaceBounds(transform2->GetWorldModelMatrix());
		float scaledRadius = dynamic_cast<ComponentCollisionSphere*>(collider)->CollisionRadius() * transform->GetBiggestScaleFactor();

		// get closest point of AABB
		glm::vec3 closestPoint = glm::vec3();
		closestPoint.x = std::max(worldSpaceBounds.minX, std::min(transform->GetWorldPosition().x, worldSpaceBounds.maxX));
		closestPoint.y = std::max(worldSpaceBounds.minY, std::min(transform->GetWorldPosition().y, worldSpaceBounds.maxY));
		closestPoint.z = std::max(worldSpaceBounds.minZ, std::min(transform->GetWorldPosition().z, worldSpaceBounds.maxZ));

		float distance = glm::distance(closestPoint, transform->GetWorldPosition());

		return distance <= scaledRadius;
	}
}