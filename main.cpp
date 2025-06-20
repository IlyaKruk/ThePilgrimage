#include "game.h"
#include "config.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");

    GameConfig::GetInstance().LoadConfig("game.cfg");

    Pilgrimage game;
    game.Run();
    return 0;
}