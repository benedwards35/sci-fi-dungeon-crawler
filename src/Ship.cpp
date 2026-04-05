#include "Ship.h"

Ship::Ship()
    : currentRoom(nullptr)
{}

Ship::~Ship() {
    for (Room* room : rooms) {
        delete room;
    }
}

void Ship::addRoom(Room* room) {
    rooms.push_back(room);

    // First room added becomes the starting room
    if (currentRoom == nullptr) {
        currentRoom = room;
    }
}

void Ship::connectRooms(Room* a, Room* b, int direction) {
    // direction is the direction you travel FROM a TO b
    // the return path is always the opposite
    int opposite = -1;
    switch (direction) {
        case NORTH: opposite = SOUTH; break;
        case SOUTH: opposite = NORTH; break;
        case EAST:  opposite = WEST;  break;
        case WEST:  opposite = EAST;  break;
        default:
            return;  // invalid direction — called before UI init, so fail silently
    }

    a->exits[direction] = b;
    b->exits[opposite]  = a;
}