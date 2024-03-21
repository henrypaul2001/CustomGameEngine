#pragma once
#include "Scene.h"
namespace Engine {
	class PhysicsScene : public Scene
	{
		bool SSAO;
		void CreateEntities();
		void CreateSystems();
	public:
		PhysicsScene(SceneManager* sceneManager);
		~PhysicsScene();

		void ChangePostProcessEffect();
		void ToggleSSAO();

		void Update() override;
		void Render() override;
		void Close() override;
		void SetupScene() override;

		void keyUp(int key) override;
		void keyDown(int key) override;
	};
}