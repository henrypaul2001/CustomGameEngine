#include "Entity.h"
namespace Engine
{
	Entity::Entity(std::string name) {
		this->name = name;
		mask = COMPONENT_NONE;
	}

	Entity::~Entity() {
		delete& name;
		delete& mask;
	}

	void Entity::AddComponent(Component* component) {
		_ASSERT(&component != nullptr, "Component cannot be null");

		componentList.push_back(component);
		mask = mask | component->ComponentType();
	}

	void Entity::Close() {
		for (Component* c : componentList) {
			c->Close();
			delete c;
		}
	}
}