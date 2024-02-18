#include "GameScene.h"
#include <iostream>
#include <glm/vec3.hpp>
#include "SystemPhysics.h"
#include <glad/glad.h>
#include "EntityManager.h"
#include "SystemManager.h"
#include "GameInputManager.h"
namespace Engine
{
	GameScene::GameScene(SceneManager* sceneManager) : Scene(sceneManager)
	{
		entityManager = new EntityManager();
		systemManager = new SystemManager();
		inputManager = new GameInputManager();

		SetupScene();
	}

	GameScene::~GameScene()
	{
		delete entityManager;
		delete systemManager;
		delete inputManager;
	}

	void GameScene::SetupScene()
	{
		// Configure GL global state
		glEnable(GL_DEPTH_TEST);

		// Set-up framebuffers

		// Compile shaders

		// Configure shaders

		// Prepare scene

		CreateSystems();
		CreateEntities();
	}

	void GameScene::CreateEntities()
	{
		Entity* rock = new Entity("Rock");
		rock->AddComponent(new ComponentTransform(0.0f, 0.0f, -10.0f));
		rock->AddComponent(new ComponentVelocity(0.25f, 0.0f, 0.0f));
		rock->AddComponent(new ComponentGeometry("Models/rock/rock.obj", false));
		entityManager->AddEntity(rock);

		Entity* rockChild = new Entity("RockChild");
		rockChild->AddComponent(new ComponentTransform(0.0f, 3.5f, 0.0f));
		rockChild->AddComponent(new ComponentGeometry("Models/rock/rock.obj", false));
		dynamic_cast<ComponentTransform*>(rockChild->GetComponent(COMPONENT_TRANSFORM))->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		dynamic_cast<ComponentTransform*>(rockChild->GetComponent(COMPONENT_TRANSFORM))->SetRotation(glm::vec3(1.0, 0.0, 0.0), 180.0f);
		dynamic_cast<ComponentTransform*>(rockChild->GetComponent(COMPONENT_TRANSFORM))->SetParent(rock);
		entityManager->AddEntity(rockChild);

		Entity* rockChild2 = new Entity("RockChild2");
		rockChild2->AddComponent(new ComponentTransform(0.0f, -10.0f, 0.0f));
		rockChild2->AddComponent(new ComponentGeometry("Models/rock/rock.obj", false));
		dynamic_cast<ComponentTransform*>(rockChild2->GetComponent(COMPONENT_TRANSFORM))->SetParent(rockChild);
		entityManager->AddEntity(rockChild2);

		Entity* rockChild3 = new Entity("RockChild3");
		rockChild3->AddComponent(new ComponentTransform(0.0f, 0.0f, -10.0f));
		rockChild3->AddComponent(new ComponentGeometry("Models/rock/rock.obj", false));
		dynamic_cast<ComponentTransform*>(rockChild3->GetComponent(COMPONENT_TRANSFORM))->SetParent(rockChild2);
		entityManager->AddEntity(rockChild3);

		Entity* backpack = new Entity("Backpack");
		backpack->AddComponent(new ComponentTransform(0.0f, 2.0f, -5.0f));
		stbi_set_flip_vertically_on_load(true);
		backpack->AddComponent(new ComponentGeometry("Models/backpack/backpack.obj", false));
		stbi_set_flip_vertically_on_load(false);
		entityManager->AddEntity(backpack);
	}

	void GameScene::CreateSystems()
	{
		systemManager->AddSystem(new SystemPhysics(), UPDATE_SYSTEMS);
		systemManager->AddSystem(new SystemRender(), RENDER_SYSTEMS);
	}

	void GameScene::Update()
	{
		systemManager->ActionUpdateSystems(entityManager);
		float time = (float)glfwGetTime();
		dynamic_cast<ComponentTransform*>(entityManager->FindEntity("Rock")->GetComponent(COMPONENT_TRANSFORM))->SetScale(glm::vec3(sin(time) * 1.0f, sin(time) * 1.0f, sin(time) * 1.0f));
		dynamic_cast<ComponentTransform*>(entityManager->FindEntity("RockChild")->GetComponent(COMPONENT_TRANSFORM))->SetRotation(glm::vec3(0.0f, 0.0f, 1.0f), sin(time) * 180.0f);
		dynamic_cast<ComponentTransform*>(entityManager->FindEntity("RockChild2")->GetComponent(COMPONENT_TRANSFORM))->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), sin(time) * 180.0f);
		dynamic_cast<ComponentTransform*>(entityManager->FindEntity("RockChild3")->GetComponent(COMPONENT_TRANSFORM))->SetRotation(glm::vec3(1.0f, 1.0f, 1.0f), sin(time) * 180.0f);
	}

	void GameScene::Render()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render entities
		systemManager->ActionRenderSystems(entityManager);
	}

	void GameScene::Close()
	{
		std::cout << "Closing game scene" << std::endl;
	}
}