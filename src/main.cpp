#include <iostream>
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

int main() {
    Player player(100, 10, 5);
    Enemy drone("Scout Drone", "A battered recon unit, one eye flickering.", 40, 10, 2);

    // Items
    Weapon plasmaCutter("Plasma Cutter", "A mining tool repurposed for combat.", 25, "plasma");
    Armor enviroSuit("Enviro Suit", "Lightweight pressure suit with reinforced plating.", 8);
    Consumable medkit("Medkit", "A sealed emergency medical kit.", 30);

    std::cout << "=== Equip Test ===\n";
    plasmaCutter.use(&player);  // equips via Weapon::use()
    enviroSuit.use(&player);    // equips via Armor::use()

    std::cout << "\n=== Combat Test ===\n";
    drone.behavior();
    player.attackTarget(&drone);
    drone.attackTarget(&player);

    std::cout << "\n=== Consumable Test ===\n";
    medkit.use(&player);

    std::cout << "\n=== Movement Test ===\n";
    player.move('w');
    player.move('d');

    std::cout << "\n=== Alive Check ===\n";
    std::cout << "Drone alive? "     << (drone.isAlive()  ? "Yes" : "No") << "\n";
    std::cout << "Commander alive? " << (player.isAlive() ? "Yes" : "No") << "\n";

    return 0;
}
