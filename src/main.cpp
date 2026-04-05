#include <iostream>
#include "Entity.h"

int main() {
    Entity marine("Marine", 100, 15, 5);
    Entity drone("Scout Drone", 40, 10, 2);

    std::cout << "=== Combat Test ===\n";
    marine.attackTarget(&drone);
    drone.attackTarget(&marine);

    std::cout << "\n--- Is the drone still alive? ---\n";
    std::cout << (drone.isAlive() ? "Yes" : "No") << "\n";

    return 0;
}