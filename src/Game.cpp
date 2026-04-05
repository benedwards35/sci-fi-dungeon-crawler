#include "Game.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

// PDCurses — comment this block out if testing without it
#include "curses.h"

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
    // --- Build the ship ---
    ship = new Ship();

    Room* bridge   = new Room("Bridge",   "Flickering consoles line the walls. A viewport stares into the void.");
    Room* corridor = new Room("Corridor", "A long maintenance corridor. Pipes hiss and groan overhead.");
    Room* medbay   = new Room("Medbay",   "Overturned gurneys. The smell of antiseptic and something worse.");
    Room* cargo    = new Room("Cargo Bay","Massive shipping containers, some torn open. Things moved in here.");

    ship->addRoom(bridge);    // first added = starting room
    ship->addRoom(corridor);
    ship->addRoom(medbay);
    ship->addRoom(cargo);

    ship->connectRooms(bridge,   corridor, SOUTH);
    ship->connectRooms(corridor, medbay,   EAST);
    ship->connectRooms(corridor, cargo,    WEST);

    // --- Populate rooms ---
    Enemy* drone = new Enemy("Scout Drone",
                             "A battered recon unit, one eye flickering.", 40, 8, 2);
    corridor->addEnemy(drone);

    Weapon* plasmaCutter = new Weapon("Plasma Cutter",
                                      "A mining tool repurposed for combat.", 25, "plasma");
    medbay->addItem(plasmaCutter);

    Armor* enviroSuit = new Armor("Enviro Suit",
                                  "Lightweight pressure suit with reinforced plating.", 8);
    cargo->addItem(enviroSuit);

    // --- Build the player ---
    player = new Player(100, 10, 5);
    player->currentRoom = ship->currentRoom;  // place player in starting room

    // --- Init PDCurses ---
    initscr();       // start curses mode
    noecho();        // don't print keypresses to screen
    cbreak();        // get keys instantly, no Enter required
    keypad(stdscr, TRUE); // enable arrow keys

    running = true;
    player->currentRoom->describe();
    refresh();
    gameLoop();

    // --- Teardown PDCurses ---
    endwin();
}

void Game::gameLoop() {
    while (running) {
        int key = getch();  // blocks until a key is pressed
        switch (key) {
            // Movement
            case KEY_UP:    case 'w': case 'W':
            case KEY_DOWN:  case 's': case 'S':
            case KEY_LEFT:  case 'a': case 'A':
            case KEY_RIGHT: case 'd': case 'D':
                player->move(key);
                if (player->currentRoom != nullptr) {
                    player->currentRoom->describe();
                    if (!player->currentRoom->enemies.empty()) {
                        handleCombat();
                    }
                }
                break;

            // Use item by index (keys 0-9)
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                player->useItem(key - '0');  // '3' - '0' = 3 (int index)
                break;

            case 'i': case 'I': {
                player->inventory->displayItems();
                refresh();
                printw("Use which item? (0-9, or any other key to cancel): ");
                refresh();
                int next = getch();
                if (next >= '0' && next <= '9') {
                    player->useItem(next - '0');
                }
                player->currentRoom->describe();
                break;
            }

            case 'p': case 'P': {
                if (player->currentRoom->items.empty()) {
                    printw("Nothing to pick up.\n");
                    refresh();
                    break;
                }
                player->currentRoom->describe();
                printw("Pick up which item? (0-9): ");
                refresh();
                int next = getch();
                if (next >= '0' && next <= '9') {
                    player->pickUpItem(next - '0');
                }
                player->currentRoom->describe();
                break;
            }

            // Quit
            case 'q': case 'Q':
                running = false;
                break;

            default:
                break;  // silently ignore unknown keys
        }

        if (!player->isAlive()) {
            printw("\nYou have died. The ship swallows you.\n");
            refresh();
            getch();  // pause so the player can read the message
            running = false;
        }
    }
}

void Game::handleCombat() {
    printw("\n--- COMBAT ---\n");
    refresh();

    while (player->isAlive() && !player->currentRoom->enemies.empty()) {
        Enemy* target = player->currentRoom->enemies[0];  // fight one at a time

        // Player attacks
        player->attackTarget(target);

        if (!target->isAlive()) {
            printw("%s is destroyed.\n", target->name.c_str());

            // Drop loot into the room
            for (Item* loot : target->lootTable) {
                player->currentRoom->addItem(loot);
                printw("%s drops to the floor.\n", loot->name.c_str());
            }
            refresh();

            player->currentRoom->enemies.erase(
                player->currentRoom->enemies.begin()
            );
            continue;
        }

        // Enemy attacks back
        target->behavior();
        target->attackTarget(player);

        // Pause so the player can read the exchange before the next round
        int key = getch();
        if (key == 'q' || key == 'Q') { running = false; break; }
    }

    if (player->isAlive()) {
        printw("--- CLEAR ---\n");
        refresh();
        printw("Press any key to continue...\n");
        refresh();
        getch();  // pause before describe() clears the screen
        player->currentRoom->describe();
    }
}
