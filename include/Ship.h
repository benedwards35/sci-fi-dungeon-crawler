#pragma once
#include <vector>
#include "Room.h"

class Ship {
public:
    std::vector<Room*> rooms;
    Room* currentRoom;    // the room the player is currently in

    Ship();
    ~Ship();              // cleans up all Room* it owns

    void addRoom(Room* room);

    // direction: use NORTH, SOUTH, EAST, WEST constants from Room.h
    // connectRooms(a, b, NORTH) means: from a, going NORTH leads to b
    //                                  from b, going SOUTH leads back to a
    void connectRooms(Room* a, Room* b, int direction);
};