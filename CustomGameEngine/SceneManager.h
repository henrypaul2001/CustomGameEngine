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
		SCENE_WIN
	};

	using SceneDelegate = std::function<void()>;

	class SceneManager
	{
	private:
		unsigned int uboMatrices;
	protected:
		void OnLoad();
		void OnUpdateFrame();
		void OnRenderFrame(Camera* camera);

		int SCR_WIDTH;
		int SCR_HEIGHT;
		int windowXPos;
		int windowYPos;
	public:
		SceneManager(int width, int height, int windowXPos, int windowYPos);
		~SceneManager();

		static Shader* defaultLit;
		Scene* scene;

		SceneDelegate renderer;
		SceneDelegate updater;

		GLFWwindow* window;

		virtual void StartNewGame() = 0;
		virtual void StartMenu() = 0;
		virtual void ChangeScene(SceneTypes sceneType) = 0;

		int* getWindowHeight() { return &SCR_HEIGHT; }
		int* getWindowWidth() { return &SCR_WIDTH; }

		void Run();
	};
}

