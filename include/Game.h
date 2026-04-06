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

    void start();        // builds the ship, places the player, calls gameLoop()
    void gameLoop();     // main loop — reads input, dispatches actions
    void handleCombat(); // turn-based fight between player and room's enemies

private:
    // Scan the current room's enemy list, log kills, drop loot, free memory.
    // Called after any action that might kill enemies without the normal
    // single-target removal path (e.g. FragGrenade AoE).
    void removeDeadEnemies();
};
