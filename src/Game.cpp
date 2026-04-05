#include "Game.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"
#include "Item.h"
#include "UI.h"

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
    // ── Build the ship ────────────────────────────────────────────────────
    ship = new Ship();

    Room* bridge   = new Room("Bridge",
        "Flickering consoles line the walls. A viewport stares into the void.");
    Room* corridor = new Room("Corridor",
        "A long maintenance corridor. Pipes hiss and groan overhead.");
    Room* medbay   = new Room("Medbay",
        "Overturned gurneys. The smell of antiseptic and something worse.");
    Room* cargo    = new Room("Cargo Bay",
        "Massive shipping containers, some torn open. Things moved in here.");

    ship->addRoom(bridge);    // first added = starting room
    ship->addRoom(corridor);
    ship->addRoom(medbay);
    ship->addRoom(cargo);

    ship->connectRooms(bridge,   corridor, SOUTH);
    ship->connectRooms(corridor, medbay,   EAST);
    ship->connectRooms(corridor, cargo,    WEST);

    // ── Populate rooms ───────────────────────────────────────────────────
    Enemy* drone = new Enemy("Scout Drone",
        "A battered recon unit, one eye flickering.", 40, 8, 2);
    corridor->addEnemy(drone);

    Weapon* plasmaCutter = new Weapon("Plasma Cutter",
        "A mining tool repurposed for combat.", 25, "plasma");
    medbay->addItem(plasmaCutter);

    Armor* enviroSuit = new Armor("Enviro Suit",
        "Lightweight pressure suit with reinforced plating.", 8);
    cargo->addItem(enviroSuit);

    // ── Build the player ─────────────────────────────────────────────────
    player = new Player(100, 10, 5);
    player->currentRoom = ship->currentRoom;

    // ── Initialise UI (starts PDCurses, creates all windows) ─────────────
    UI::get().init();

    running = true;
    UI::get().drawStats(player);
    UI::get().drawRoom(player->currentRoom);

    gameLoop();

    UI::get().shutdown();
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
            case KEY_RIGHT: case 'd': case 'D':
                player->move(key);
                if (player->currentRoom != nullptr) {
                    UI::get().drawRoom(player->currentRoom);
                    if (!player->currentRoom->enemies.empty()) {
                        handleCombat();
                    }
                }
                break;

            // ── Use item by index (0–9) ───────────────────────────────────
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                player->useItem(key - '0');
                break;

            // ── Inventory ────────────────────────────────────────────────
            case 'i': case 'I': {
                player->inventory->displayItems();
                UI::get().message("Use which item? (0-9, or any other key to cancel)");
                int next = UI::get().getInput();
                if (next >= '0' && next <= '9') {
                    player->useItem(next - '0');
                }
                UI::get().message("");
                break;
            }

            // ── Pick up ───────────────────────────────────────────────────
            case 'p': case 'P': {
                if (player->currentRoom->items.empty()) {
                    UI::get().message("Nothing to pick up.");
                    break;
                }
                UI::get().message("Pick up which item? (0-9):");
                int next = UI::get().getInput();
                if (next >= '0' && next <= '9') {
                    player->pickUpItem(next - '0');
                }
                UI::get().drawRoom(player->currentRoom);
                break;
            }

            // ── Quit ─────────────────────────────────────────────────────
            case 'q': case 'Q':
                running = false;
                break;

            default:
                break;  // silently ignore unknown keys
        }

        if (!player->isAlive()) {
            UI::get().log(UI::STYLE_ENEMY, "You have died. The ship swallows you.");
            UI::get().message("Press any key to exit.");
            UI::get().getInput();
            running = false;
        }
    }
}

void Game::handleCombat() {
    UI::get().log(UI::STYLE_HEADER, "");
    UI::get().log(UI::STYLE_HEADER, "=== COMBAT INITIATED ===");

    while (player->isAlive() && !player->currentRoom->enemies.empty()) {
        Enemy* target = player->currentRoom->enemies[0];

        // Player attacks
        player->attackTarget(target);
        UI::get().drawStats(player);

        if (!target->isAlive()) {
            UI::get().log(UI::STYLE_GOOD, "%s is destroyed.", target->name.c_str());

            for (Item* loot : target->lootTable) {
                player->currentRoom->addItem(loot);
                UI::get().log(UI::STYLE_ITEM, "%s drops to the floor.", loot->name.c_str());
            }

            player->currentRoom->enemies.erase(
                player->currentRoom->enemies.begin()
            );
            continue;
        }

        // Enemy attacks back
        target->behavior();
        target->attackTarget(player);
        UI::get().drawStats(player);

        // Pause between rounds so the player can read the exchange
        UI::get().message("Press any key for next round, [Q] to flee.");
        int key = UI::get().getInput();
        if (key == 'q' || key == 'Q') { running = false; break; }
        UI::get().message("");
    }

    if (player->isAlive()) {
        UI::get().log(UI::STYLE_GOOD, "=== AREA CLEAR ===");
        UI::get().message("Combat over. Press any key to continue.");
        UI::get().getInput();
        UI::get().message("");
        UI::get().drawRoom(player->currentRoom);
    }
}
