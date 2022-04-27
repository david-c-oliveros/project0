#include "Game.h"


int main ()
{
    Game game = Game();

    if (game.Construct())
        game.Start();

    return 0;
}
