﻿#include "game.h"
#include "config.h"

int main() {
    GameConfig::GetInstance().LoadConfig("game.cfg");
    Pilgrimage game;
    game.Run();
    return 0;
}