#pragma once
#include <functional>
namespace Engine
{
	class Scene;

	enum SceneTypes {
		SCENE_NONE,
		SCENE_MAIN_MENU,
		SCENE_GAME,
		SCENE_GAME_OVER,
		SCENE_WIN
	};

	using SceneDelegate = std::function<void()>;
	using KeyboardDelegate = std::function<void()>;
	using MouseDelegate = std::function<void()>;

	class SceneManager
	{
	protected:
		void OnLoadTemp();
		void OnUpdateFrame();
		void OnRenderFrame();

		//bool gameIsRunning = false;
	public:
		SceneManager(int width, int height, int windowXPos, int windowYPos);
		~SceneManager();

		static int width;
		static int height;
		static int windowXPos;
		static int windowYPos;

		Scene* scene;

		SceneDelegate renderer;
		SceneDelegate updater;

		KeyboardDelegate keyboardDownDelegate;
		KeyboardDelegate keyboardUpDelegate;

		MouseDelegate mouseDelegate;

		virtual void StartNewGame() = 0;
		virtual void StartMenu() = 0;
		virtual void ChangeScene(SceneTypes sceneType) = 0;

		static int& getWindowHeight() { return height; }
		static int& getWindowWidth() { return width; }

		void Run();
	};
}

