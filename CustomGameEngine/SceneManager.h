#pragma once
#include <functional>
#include "Scene.h"
#include <thread>
#include <iostream>
namespace Engine
{
	enum SceneTypes {
		SCENE_NONE,
		SCENE_MAIN_MENU,
		SCENE_GAME,
		SCENE_GAME_OVER,
		SCENE_WIN,
		SCENE_SPONZA,
		SCENE_AO,
		SCENE_PBR,
		SCENE_COLLISIONS,
		SCENE_PHYSICS
	};

	using SceneDelegate = std::function<void()>;

	class SceneManager
	{
	protected:
		void OnLoad();
		void OnUpdateFrame();
		void OnRenderFrame();

		ResourceManager* resources;

		int SCR_WIDTH;
		int SCR_HEIGHT;
		int windowXPos;
		int windowYPos;
	public:
		SceneManager(int width, int height, int windowXPos, int windowYPos);
		~SceneManager();

		Scene* scene;

		SceneDelegate renderer;
		SceneDelegate updater;

		GLFWwindow* window;

		virtual void StartNewGame() = 0;
		virtual void StartMenu() = 0;
		virtual void ChangeScene(SceneTypes sceneType) = 0;

		int GetWindowHeight() { return SCR_HEIGHT; }
		int GetWindowWidth() { return SCR_WIDTH; }

		void Run();
	};
}

