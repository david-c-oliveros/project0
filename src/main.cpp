#include "Game.h"


int main ()
{
    Game game = Game(1920, 1080);

    if (game.Construct())
        game.Start();

    return 0;
}
