#include "SystemRender.h"

namespace Engine {
	SystemRender::SystemRender()
	{

	}

	SystemRender::~SystemRender()
	{

	}

	void SystemRender::OnAction(Entity* entity)
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

			ComponentGeometry* geometry = nullptr;
			for (Component* c : components) {
				geometry = dynamic_cast<ComponentGeometry*>(c);
				if (geometry != nullptr) {
					break;
				}
			}

			//std::cout << entity->Name();
			Draw(transform, geometry);
		}
	}

	void SystemRender::Draw(ComponentTransform* transform, ComponentGeometry* geometry)
	{
		Shader* shader = geometry->GetShader();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform->Position());
		model = glm::scale(model, transform->Scale());
		model = glm::rotate(model, glm::radians(transform->RotationAngle()), transform->RotationAxis());
		shader->setMat4("model", model);
		shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		shader->setFloat("textureScale", 1.0f);
		geometry->GetModel()->Draw(*geometry->GetShader());
	}
}