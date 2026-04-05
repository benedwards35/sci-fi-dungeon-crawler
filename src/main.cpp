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

    Weapon plasmaCutter("Plasma Cutter", "A mining tool repurposed for combat.", 25, "plasma");
    Armor enviroSuit("Enviro Suit", "Lightweight pressure suit with reinforced plating.", 8);
    Consumable medkit("Medkit", "A sealed emergency medical kit.", 30);
    Consumable stimpack("Stimpack", "Military-grade adrenaline shot.", 15);

    std::cout << "=== Inventory Test ===\n";
    player.inventory->addItem(&plasmaCutter);
    player.inventory->addItem(&enviroSuit);
    player.inventory->addItem(&medkit);
    player.inventory->addItem(&stimpack);
    player.inventory->displayItems();

    std::cout << "\n=== Equip From Inventory ===\n";
    player.useItem(0);  // equips Plasma Cutter (stays in inventory)
    player.useItem(1);  // equips Enviro Suit (stays in inventory)
    player.inventory->displayItems();

    std::cout << "\n=== Combat Test ===\n";
    drone.behavior();
    player.attackTarget(&drone);
    drone.attackTarget(&player);

    std::cout << "\n=== Consumable Test ===\n";
    std::cout << "HP before: " << player.hp << "/" << player.maxHp << "\n";
    player.useItem(2);  // uses Medkit — heals and auto-removes from inventory
    player.inventory->displayItems();

    std::cout << "\n=== Has Item Test ===\n";
    std::cout << "Has stimpack? " << (player.inventory->hasItem(&stimpack) ? "Yes" : "No") << "\n";
    std::cout << "Has medkit?   " << (player.inventory->hasItem(&medkit)   ? "Yes" : "No") << "\n";

    return 0;
}