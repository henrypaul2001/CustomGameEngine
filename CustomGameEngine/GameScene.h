#pragma once
#include "Scene.h"
#include "EntityManager.h"
#include "SystemManager.h"
namespace Engine
{
	class GameScene : public Scene
	{
	private:
		EntityManager entityManager;
		SystemManager systemManager;

		void CreateEntities();
		void CreateSystems();
	public:
		GameScene(SceneManager* sceneManager);
		~GameScene();

		void Update() override;
		void Render() override;
		void Close() override;
	};
}