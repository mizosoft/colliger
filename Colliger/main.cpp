#include "Game.h"
#include "Screen/LoadingScreen.h"
#include "Entity/ColligerAnimationData.h"
#include "Entity/GhostAnimationData.h"

#include <iostream>
#include <cstdlib>

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;
	game.SetScreen(std::make_shared<LoadingScreen>(game));
	game.Run();
}
