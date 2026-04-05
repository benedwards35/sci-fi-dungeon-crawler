#include <iostream>
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

int main() {
    Player player(100, 15, 5);
    Enemy drone("Scout Drone", "A battered recon unit, one eye flickering.", 40, 10, 2);

    std::cout << "=== Behavior Test ===\n";
    drone.behavior();

    std::cout << "\n=== Combat Test ===\n";
    player.attackTarget(&drone);
    drone.attackTarget(&player);

    std::cout << "\n=== Movement Test ===\n";
    player.move('w');
    player.move('d');

    std::cout << "\n=== Alive Check ===\n";
    std::cout << "Drone alive? " << (drone.isAlive() ? "Yes" : "No") << "\n";
    std::cout << "Commander alive? " << (player.isAlive() ? "Yes" : "No") << "\n";

    return 0;
}