#pragma once
#include "SystemCollision.h"
#include "ComponentTransform.h"
#include "ComponentCollisionBox.h"
namespace Engine {
    class SystemCollisionBox : public SystemCollision
    {
	private:
		const ComponentTypes MASK = (COMPONENT_TRANSFORM | COMPONENT_COLLISION_BOX);

		bool Intersect(ComponentTransform* transform, ComponentCollision* collider, ComponentTransform* transform2, ComponentCollision* collider2) override;
	public:
		SystemCollisionBox(EntityManager* entityManager);
		~SystemCollisionBox();

		SystemTypes Name() override { return SYSTEM_COLLISION_BOX; }
		void OnAction(Entity* entity) override;
		void AfterAction() override;
    };
}