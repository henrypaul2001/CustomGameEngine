#include "GameSceneManager.h"

int main()
{
	Engine::GameSceneManager game = Engine::GameSceneManager(1200, 800, 200, 80);
	game.Run();

	return 0;
}