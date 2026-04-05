#pragma once
#include <deque>
#include <string>
#include "curses.h"

class Player;
class Room;
class Inventory;

// UI is a singleton that owns all PDCurses windows.
// Game logic calls UI::get().log() / UI::get().message() instead of printw().
class UI {
public:
    // Style constants for log()
    static const int STYLE_NORMAL = 0;  // white  — default
    static const int STYLE_HEADER = 1;  // bold cyan — room names, section headers
    static const int STYLE_ENEMY  = 2;  // red    — enemy actions / danger
    static const int STYLE_ITEM   = 3;  // yellow — items, pickups
    static const int STYLE_GOOD   = 4;  // green  — positive outcomes

    static UI& get();

    // Initialise PDCurses, create all windows and draw static chrome.
    void init();

    // Tear down all windows and call endwin().
    void shutdown();

    // Append a formatted line to the scrolling main log and redraw.
    void log(const char* fmt, ...);
    void log(int style, const char* fmt, ...);

    // Replace the single line in the message bar.
    void message(const char* fmt, ...);

    // Redraw the entire stats panel from the player's current state.
    void drawStats(const Player* player);

    // Push a full room description into the main log.
    void drawRoom(const Room* room);

    // Push the inventory listing into the main log.
    void drawInventory(const Inventory* inv);

    // Block until a key is pressed; returns the PDCurses key code.
    int getInput();

private:
    UI();

    struct LogLine {
        std::string text;
        int         style;
    };

    WINDOW* mainWin;
    WINDOW* statsWin;
    WINDOW* msgWin;
    WINDOW* hintWin;

    int mainH, mainW, statsW, msgH;

    std::deque<LogLine> logLines;
    static const int MAX_LOG = 500;

    void redrawMain();
    void redrawHint();
    void applyStyle(WINDOW* win, int style);
    void clearStyle(WINDOW* win, int style);
};
