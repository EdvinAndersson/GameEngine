#include <stdio.h>
#include "Game.h"

int main() {
    CWGame::Game game = CWGame::Game();
    game.Init();
    game.Run();

    return 0;
}