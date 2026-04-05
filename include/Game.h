#pragma once
#include "Player.h"
#include "Ship.h"

class Game {
public:
    Player* player;
    Ship*   ship;
    bool    running;

    Game();
    ~Game();

    void start();       // builds the ship, places the player, calls gameLoop()
    void gameLoop();    // main loop — reads input, dispatches actions
    void handleCombat();// turn-based fight between player and room's enemies
};