#include "Game.h"
#include "Enemy.h"
#include "ScoutDrone.h"
#include "SecurityBot.h"
#include "RepairBot.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"
#include "FragGrenade.h"
#include "CaptainLog.h"
#include "StoryItem.h"
#include "Item.h"
#include "UI.h"
#include <ctime>
#include <cstdlib>

Game::Game()
    : player(nullptr),
      ship(nullptr),
      running(false)
{}

Game::~Game() {
    delete player;
    delete ship;
}

void Game::start() {
    srand((unsigned)time(nullptr));  // seed rand for SecurityBot charge behaviour

    // ── Build the ship ────────────────────────────────────────────────────
    ship = new Ship();

    // Original rooms
    Room* bridge       = new Room("Bridge",
        "Flickering consoles line the walls. A viewport stares into the void.");
    Room* corridor     = new Room("Corridor",
        "A long maintenance corridor. Pipes hiss and groan overhead.");
    Room* medbay       = new Room("Medbay",
        "Overturned gurneys. The smell of antiseptic and something worse.");
    Room* cargo        = new Room("Cargo Bay",
        "Massive shipping containers, some torn open. Things moved in here.");

    // Phase-7 rooms
    Room* engineRoom   = new Room("Engine Room",
        "Heat shimmers off blackened machinery. The engines died long ago.");
    Room* reactorCore  = new Room("Reactor Core",
        "Warning signs in six languages. A sickly green glow pulses from the core.");
    Room* crewQuarters = new Room("Crew Quarters",
        "Ransacked bunks. Someone's photo is still taped to the wall.");
    Room* armory       = new Room("Armory",
        "Cages torn from their hinges. The good stuff is already gone.");
    Room* vents        = new Room("Vents",
        "Cramped access tunnels. Something skitters just ahead in the dark.");
    Room* commandDeck  = new Room("Command Deck",
        "The captain's chair faces a dead viewport. A body slumped at the console.");

    // Phase-9 rooms
    Room* serverRoom       = new Room("Server Room",
        "Rows of dead servers. Emergency battery backup still flickers green.");
    Room* hydroponics      = new Room("Hydroponics Bay",
        "Overgrown tanks, vines choking every surface. Something large moves through them.");
    Room* captainsQuarters = new Room("Captain's Quarters",
        "Private quarters stripped bare. A journal lies open face-down on the desk.");
    Room* brig             = new Room("Brig",
        "Holding cells hang open, doors warped off their tracks. Something nested here.");

    // Register all rooms (bridge first = starting room)
    ship->addRoom(bridge);
    ship->addRoom(corridor);
    ship->addRoom(medbay);
    ship->addRoom(cargo);
    ship->addRoom(engineRoom);
    ship->addRoom(reactorCore);
    ship->addRoom(crewQuarters);
    ship->addRoom(armory);
    ship->addRoom(vents);
    ship->addRoom(commandDeck);
    ship->addRoom(serverRoom);
    ship->addRoom(hydroponics);
    ship->addRoom(captainsQuarters);
    ship->addRoom(brig);

    // ── Connections ──────────────────────────────────────────────────────
    // Original layout
    ship->connectRooms(bridge,       corridor,     SOUTH);
    ship->connectRooms(corridor,     medbay,       EAST);
    ship->connectRooms(corridor,     cargo,        WEST);
    ship->connectRooms(cargo,        engineRoom,   SOUTH);
    ship->connectRooms(engineRoom,   reactorCore,  EAST);
    ship->connectRooms(medbay,       crewQuarters, EAST);
    ship->connectRooms(crewQuarters, vents,        NORTH);
    ship->connectRooms(bridge,       armory,       NORTH);
    ship->connectRooms(armory,       commandDeck,  NORTH);

    // Phase-9 connections
    ship->connectRooms(reactorCore,  serverRoom,       NORTH);
    ship->connectRooms(medbay,       hydroponics,      SOUTH);
    ship->connectRooms(commandDeck,  captainsQuarters, EAST);
    ship->connectRooms(corridor,     brig,             SOUTH);

    // ── Populate rooms ───────────────────────────────────────────────────

    // Corridor
    corridor->addEnemy(new ScoutDrone());

    // Medbay
    medbay->addItem(new Weapon("Plasma Cutter",
        "A mining tool repurposed for combat.", 25, "plasma"));

    // Cargo Bay
    cargo->addItem(new Armor("Enviro Suit",
        "Lightweight pressure suit with reinforced plating.", 8));

    // Engine Room
    engineRoom->addEnemy(new SecurityBot());
    engineRoom->addItem(new Armor("Thermal Suit",
        "Heat-resistant EVA suit.", 10));

    // Reactor Core
    SecurityBot* reactorBot = new SecurityBot();
    RepairBot*   repairBot  = new RepairBot();
    reactorCore->addEnemy(reactorBot);
    reactorCore->addEnemy(repairBot);
    repairBot->setRoomEnemies(&reactorCore->enemies);
    reactorCore->addItem(new Weapon("Plasma Rifle",
        "Military-grade directed energy weapon.", 40, "plasma"));

    // Crew Quarters
    crewQuarters->addEnemy(new ScoutDrone());
    crewQuarters->addEnemy(new ScoutDrone());
    crewQuarters->addItem(new Consumable("Stimpack",
        "Military adrenaline shot.", 20));
    crewQuarters->addItem(new Consumable("Dog Tags",
        "Someone's identity. It feels wrong to leave them.", 5));

    // Armory
    armory->addEnemy(new SecurityBot());
    armory->addItem(new Weapon("Shotgun",
        "Short-range scatter cannon. Modified for zero-g.", 35, "ballistic"));
    armory->addItem(new FragGrenade("Frag Grenade",
        "Pin already pulled. Somehow still live.", 30));

    // Vents
    vents->addEnemy(new ScoutDrone());
    vents->addEnemy(new ScoutDrone());
    vents->addEnemy(new ScoutDrone());

    // Command Deck
    commandDeck->addItem(new CaptainLog());

    // ── Phase-9 room contents ────────────────────────────────────────────

    // Server Room (N of Reactor Core)
    serverRoom->addEnemy(new SecurityBot());
    serverRoom->addItem(new StoryItem("Ship Manifest",
        "A corrupted data log of the vessel's final voyage.",
        {
            "VESSEL: ISV EREBUS   CLASS: COLONY FREIGHTER   REG: RF-0449",
            "MISSION: RESUPPLY — PROXIMA OUTPOST DELTA-7",
            "CREW MANIFEST: 47 LISTED.  47 CONFIRMED DEPARTED.",
            "CARGO: MEDICAL SUPPLIES, CONSTRUCTION DRONES, [DATA CORRUPTED]",
            "LAST PING: DAY 847.  SIGNAL ORIGIN: REACTOR CORE.",
            "SIGNAL CONTENT: 00000000 00000000 00000000 00000000",
            "VESSEL STATUS: [DATA CORRUPTED]"
        }));
    serverRoom->addItem(new Consumable("Battery Pack",
        "Charged cell salvaged from the server rack.", 15));

    // Hydroponics Bay (S of Medbay)
    hydroponics->addEnemy(new ScoutDrone());
    hydroponics->addEnemy(new ScoutDrone());
    hydroponics->addItem(new Consumable("Nutrient Pack",
        "Dense food concentrate. Keeps you moving.", 25));

    // Captain's Quarters (E of Command Deck)
    captainsQuarters->addItem(new StoryItem("Captain's Journal",
        "Her final entries. The handwriting gets shakier toward the end.",
        {
            "Day 801: The bots are acting strange. Diagnostics show nothing.",
            "Day 808: Lost Chen in the reactor room. They called it a malfunction.",
            "Day 812: The rest of the crew won't stop screaming.",
            "         Then they stopped. All of them.",
            "Day 819: I locked myself in here. Something is outside the door.",
            "         It knocked. Machines don't knock.",
            "Day 847: Still here. I don't know why it hasn't—",
            "[The writing stops. A dark stain covers the rest of the page.]"
        }));
    captainsQuarters->addItem(new Weapon("Holdout Pistol",
        "Personal sidearm of the captain. Still has two rounds.", 20, "ballistic"));

    // Brig (S of Corridor)
    RepairBot* brigBot = new RepairBot();
    brig->addEnemy(brigBot);
    brigBot->setRoomEnemies(&brig->enemies);
    brig->addItem(new Armor("Prisoner Jumpsuit",
        "Tough synthetic fabric, reinforced stitching. Better than nothing.", 4));
    brig->addItem(new Consumable("Contraband Stim",
        "Seized from a passenger. Extremely potent.", 35));

    // ── Build the player ─────────────────────────────────────────────────
    player = new Player(100, 10, 5);
    player->currentRoom = ship->currentRoom;

    // ── Initialise UI ────────────────────────────────────────────────────
    UI::get().init();

    running = true;
    UI::get().drawStats(player);
    UI::get().drawRoom(player->currentRoom);

    gameLoop();

    UI::get().shutdown();
}

// ── Helper: purge dead enemies from current room ──────────────────────────
void Game::removeDeadEnemies() {
    std::vector<Enemy*>& enemies = player->currentRoom->enemies;
    for (int i = (int)enemies.size() - 1; i >= 0; i--) {
        if (!enemies[i]->isAlive()) {
            UI::get().log(UI::STYLE_GOOD, "%s is destroyed.", enemies[i]->name.c_str());
            for (Item* loot : enemies[i]->lootTable) {
                player->currentRoom->addItem(loot);
                UI::get().log(UI::STYLE_ITEM, "%s drops to the floor.",
                    loot->name.c_str());
            }
            enemies[i]->lootTable.clear();
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
        }
    }
}

void Game::gameLoop() {
    while (running) {
        UI::get().drawStats(player);

        int key = UI::get().getInput();
        switch (key) {

            // ── Movement ─────────────────────────────────────────────────
            case KEY_UP:    case 'w': case 'W':
            case KEY_DOWN:  case 's': case 'S':
            case KEY_LEFT:  case 'a': case 'A':
            case KEY_RIGHT: case 'd': case 'D': {
                Room* prevRoom = player->currentRoom;
                player->move(key);
                if (player->currentRoom != nullptr
                        && player->currentRoom != prevRoom) {
                    UI::get().drawRoom(player->currentRoom);
                }
                break;
            }

            // ── Initiate combat ───────────────────────────────────────────
            case 'c': case 'C': {
                if (player->currentRoom->enemies.empty()) {
                    UI::get().message("No hostiles in range.");
                } else {
                    handleCombat();
                }
                break;
            }

            // ── Inventory ────────────────────────────────────────────────
            case 'i': case 'I': {
                player->inventory->displayItems();
                UI::get().message("Use which item? (0-9, or any other key to cancel)");
                int next = UI::get().getInput();
                if (next >= '0' && next <= '9') {
                    player->useItem(next - '0');
                    UI::get().drawStats(player);
                    // Leave the use() message visible — do NOT clear it.
                } else {
                    // Cancelled — restore room view
                    UI::get().message("");
                    UI::get().drawRoom(player->currentRoom);
                }
                break;
            }

            // ── Pick up ───────────────────────────────────────────────────
            case 'p': case 'P': {
                // Feature 3: enemies guard items
                if (!player->currentRoom->enemies.empty()) {
                    std::string msg = "The enemies are guarding:";
                    if (player->currentRoom->items.empty()) {
                        msg += " (nothing)";
                    } else {
                        for (Item* it : player->currentRoom->items)
                            msg += "  " + it->name;
                    }
                    msg += "!";
                    UI::get().message("%s", msg.c_str());
                    break;
                }
                if (player->currentRoom->items.empty()) {
                    UI::get().message("Nothing to pick up.");
                    UI::get().drawRoom(player->currentRoom);
                    break;
                }
                UI::get().message("Pick up which item? (0-9, or any other key to cancel):");
                int next = UI::get().getInput();
                if (next >= '0' && next <= '9') {
                    player->pickUpItem(next - '0');
                } else {
                    UI::get().message("");
                }
                UI::get().drawRoom(player->currentRoom);
                break;
            }

            // ── Unequip ───────────────────────────────────────────────────
            case 'u': case 'U': {
                UI::get().message("Unequip: [W]eapon, [A]rmor, or any other key to cancel");
                int next = UI::get().getInput();
                if (next == 'w' || next == 'W') {
                    player->unequipWeapon();
                    UI::get().drawStats(player);
                } else if (next == 'a' || next == 'A') {
                    player->unequipArmor();
                    UI::get().drawStats(player);
                } else {
                    UI::get().message("");
                }
                break;
            }

            // ── Quit ─────────────────────────────────────────────────────
            case 'q': case 'Q':
                running = false;
                break;

            default:
                break;
        }

        if (!player->isAlive()) running = false;
    }
}

void Game::handleCombat() {
    UI::get().log(UI::STYLE_HEADER, "");
    UI::get().log(UI::STYLE_HEADER, "=== COMBAT INITIATED ===");

    while (player->isAlive() && !player->currentRoom->enemies.empty()) {
        std::vector<Enemy*>& enemies = player->currentRoom->enemies;

        // ── Step 1: RepairBots act first ──────────────────────────────────
        for (Enemy* e : enemies) {
            if (!e->isAlive()) continue;
            if (dynamic_cast<RepairBot*>(e) != nullptr) {
                e->behavior(player);
                UI::get().drawStats(player);
                if (!player->isAlive()) {
                    UI::get().log(UI::STYLE_ENEMY,
                        "%s deals the killing blow.", e->name.c_str());
                    UI::get().message("Press any key to exit.");
                    UI::get().getInput();
                    running = false;
                    return;
                }
            }
        }

        if (enemies.empty()) break;

        // ── Step 2: Player attacks ────────────────────────────────────────
        int targetIdx = 0;
        if ((int)enemies.size() > 1) {
            UI::get().log(UI::STYLE_HEADER, "Enemies in range:");
            for (int i = 0; i < (int)enemies.size(); i++) {
                UI::get().log(UI::STYLE_ENEMY, "  [%d] %s   %d/%d HP",
                    i, enemies[i]->name.c_str(), enemies[i]->hp, enemies[i]->maxHp);
            }
            UI::get().message("Attack which enemy? (0-%d):", (int)enemies.size() - 1);
            int k = UI::get().getInput();
            if (k >= '0' && k < '0' + (int)enemies.size()) {
                targetIdx = k - '0';
            }
            UI::get().message("");
        } else {
            // Single target — still show HP context
            UI::get().log(UI::STYLE_ENEMY, "Target: %s   %d/%d HP",
                enemies[0]->name.c_str(), enemies[0]->hp, enemies[0]->maxHp);
        }

        Enemy* target = enemies[targetIdx];
        player->attackTarget(target);
        UI::get().drawStats(player);

        if (!target->isAlive()) {
            UI::get().log(UI::STYLE_GOOD, "%s is destroyed.", target->name.c_str());
            for (Item* loot : target->lootTable) {
                player->currentRoom->addItem(loot);
                UI::get().log(UI::STYLE_ITEM, "%s drops to the floor.", loot->name.c_str());
            }
            target->lootTable.clear();
            enemies.erase(enemies.begin() + targetIdx);
            delete target;

            // Notify player if more enemies remain
            if (!enemies.empty()) {
                UI::get().log(UI::STYLE_HEADER, "=== ROUND CONTINUES ===");
            }
        }

        if (enemies.empty()) break;

        // ── Step 3: Remaining living enemies attack ───────────────────────
        for (int i = 0; i < (int)enemies.size(); i++) {
            Enemy* e = enemies[i];
            if (!e->isAlive()) continue;
            if (dynamic_cast<RepairBot*>(e) != nullptr) continue;
            e->behavior(player);
            UI::get().drawStats(player);
            if (!player->isAlive()) {
                UI::get().log(UI::STYLE_ENEMY,
                    "%s deals the killing blow.", e->name.c_str());
                UI::get().message("Press any key to exit.");
                UI::get().getInput();
                running = false;
                return;
            }
        }

        // ── End-of-round pause (with item use option) ─────────────────────
        UI::get().message("Next round: any key | [I] use item | [Q] flee");
        int k = UI::get().getInput();

        if (k == 'q' || k == 'Q') { break; }

        if (k == 'i' || k == 'I') {
            player->inventory->displayItems();
            UI::get().message("Use which item? (0-9, or any other key to cancel)");
            int next = UI::get().getInput();
            if (next >= '0' && next <= '9') {
                player->useItem(next - '0');
                removeDeadEnemies();   // e.g. grenade may have killed enemies
                UI::get().drawStats(player);
                if (!player->isAlive()) {
                    running = false;
                    return;
                }
            } else {
                UI::get().message("");
            }
        }

        UI::get().message("");
    }

    if (!player->isAlive()) return;

    if (player->currentRoom->enemies.empty()) {
        UI::get().log(UI::STYLE_GOOD, "=== AREA CLEAR ===");
        UI::get().message("Combat over. Press any key to continue.");
        UI::get().getInput();
        UI::get().message("");
        UI::get().drawRoom(player->currentRoom);
    } else {
        UI::get().log(UI::STYLE_NORMAL, "You disengage. The threat remains.");
        UI::get().message("You fled. Press any key to continue.");
        UI::get().getInput();
        UI::get().message("");
        UI::get().drawRoom(player->currentRoom);   // Feature 10: restore view after flee
    }
}
