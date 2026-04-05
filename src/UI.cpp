#include "UI.h"
#include "Player.h"
#include "Weapon.h"
#include "Armor.h"
#include "Room.h"
#include "Enemy.h"
#include "Item.h"
#include "Inventory.h"
#include <cstdarg>
#include <cstdio>
#include <string>

// ── Color pair indices (private to this file) ────────────────────────────
static const int CP_BORDER = 1;   // cyan  — all window borders
static const int CP_ROOM   = 2;   // cyan  — room/section headers
static const int CP_NORMAL = 3;   // white — default text
static const int CP_ENEMY  = 4;   // red   — enemies / danger
static const int CP_ITEM   = 5;   // yellow— items
static const int CP_GOOD   = 6;   // green — positive events
static const int CP_STATS  = 7;   // magenta — stats panel title
static const int CP_MSG    = 8;   // yellow — message bar
static const int CP_HINT   = 9;   // white  — hint bar

// ── Singleton ────────────────────────────────────────────────────────────

UI::UI()
    : mainWin(nullptr), statsWin(nullptr), msgWin(nullptr), hintWin(nullptr),
      mainH(0), mainW(0), statsW(22), msgH(3)
{}

UI& UI::get() {
    static UI instance;
    return instance;
}

// ── Init / shutdown ───────────────────────────────────────────────────────

void UI::init() {
    initscr();
    noecho();
    cbreak();

    statsW = 22;
    msgH   = 3;
    mainH  = LINES - msgH - 1;   // reserve 3 rows for msg box + 1 for hint bar
    mainW  = COLS  - statsW;

    if (has_colors()) {
        start_color();
        init_pair(CP_BORDER, COLOR_CYAN,    COLOR_BLACK);
        init_pair(CP_ROOM,   COLOR_CYAN,    COLOR_BLACK);
        init_pair(CP_NORMAL, COLOR_WHITE,   COLOR_BLACK);
        init_pair(CP_ENEMY,  COLOR_RED,     COLOR_BLACK);
        init_pair(CP_ITEM,   COLOR_YELLOW,  COLOR_BLACK);
        init_pair(CP_GOOD,   COLOR_GREEN,   COLOR_BLACK);
        init_pair(CP_STATS,  COLOR_MAGENTA, COLOR_BLACK);
        init_pair(CP_MSG,    COLOR_YELLOW,  COLOR_BLACK);
        init_pair(CP_HINT,   COLOR_WHITE,   COLOR_BLACK);
    }

    mainWin  = newwin(mainH, mainW,  0,          0     );
    statsWin = newwin(mainH, statsW, 0,          mainW );
    msgWin   = newwin(msgH,  COLS,   mainH,      0     );
    hintWin  = newwin(1,     COLS,   mainH+msgH, 0     );

    keypad(mainWin, TRUE);   // arrow keys work through wgetch(mainWin)

    // Draw initial chrome
    redrawMain();

    werase(statsWin);
    wattron(statsWin, COLOR_PAIR(CP_BORDER));
    box(statsWin, 0, 0);
    wattroff(statsWin, COLOR_PAIR(CP_BORDER));
    wrefresh(statsWin);

    werase(msgWin);
    wattron(msgWin, COLOR_PAIR(CP_BORDER));
    box(msgWin, 0, 0);
    wattroff(msgWin, COLOR_PAIR(CP_BORDER));
    wrefresh(msgWin);

    redrawHint();
}

void UI::shutdown() {
    if (mainWin)  { delwin(mainWin);  mainWin  = nullptr; }
    if (statsWin) { delwin(statsWin); statsWin = nullptr; }
    if (msgWin)   { delwin(msgWin);   msgWin   = nullptr; }
    if (hintWin)  { delwin(hintWin);  hintWin  = nullptr; }
    endwin();
}

// ── Style helpers ─────────────────────────────────────────────────────────

void UI::applyStyle(WINDOW* win, int style) {
    switch (style) {
        case STYLE_HEADER: wattron(win, COLOR_PAIR(CP_ROOM)  | A_BOLD); break;
        case STYLE_ENEMY:  wattron(win, COLOR_PAIR(CP_ENEMY));           break;
        case STYLE_ITEM:   wattron(win, COLOR_PAIR(CP_ITEM));            break;
        case STYLE_GOOD:   wattron(win, COLOR_PAIR(CP_GOOD));            break;
        default:           wattron(win, COLOR_PAIR(CP_NORMAL));          break;
    }
}

void UI::clearStyle(WINDOW* win, int style) {
    switch (style) {
        case STYLE_HEADER: wattroff(win, COLOR_PAIR(CP_ROOM)  | A_BOLD); break;
        case STYLE_ENEMY:  wattroff(win, COLOR_PAIR(CP_ENEMY));           break;
        case STYLE_ITEM:   wattroff(win, COLOR_PAIR(CP_ITEM));            break;
        case STYLE_GOOD:   wattroff(win, COLOR_PAIR(CP_GOOD));            break;
        default:           wattroff(win, COLOR_PAIR(CP_NORMAL));          break;
    }
}

// ── Log ───────────────────────────────────────────────────────────────────

// Shared formatting helper
static std::string vfmt(const char* fmt, va_list args) {
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    return std::string(buf);
}

void UI::log(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::string text = vfmt(fmt, args);
    va_end(args);
    logLines.push_back({text, STYLE_NORMAL});
    while ((int)logLines.size() > MAX_LOG) logLines.pop_front();
    redrawMain();
}

void UI::log(int style, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::string text = vfmt(fmt, args);
    va_end(args);
    logLines.push_back({text, style});
    while ((int)logLines.size() > MAX_LOG) logLines.pop_front();
    redrawMain();
}

void UI::redrawMain() {
    if (!mainWin) return;

    werase(mainWin);
    wattron(mainWin, COLOR_PAIR(CP_BORDER));
    box(mainWin, 0, 0);
    wattroff(mainWin, COLOR_PAIR(CP_BORDER));

    int innerH = mainH - 2;
    int innerW = mainW - 2;
    int start  = ((int)logLines.size() > innerH)
                    ? (int)logLines.size() - innerH : 0;

    for (int i = start; i < (int)logLines.size(); i++) {
        const LogLine& ll = logLines[i];
        int row = 1 + (i - start);
        applyStyle(mainWin, ll.style);
        std::string text = ll.text;
        if ((int)text.size() > innerW) text = text.substr(0, innerW);
        mvwprintw(mainWin, row, 1, "%s", text.c_str());
        clearStyle(mainWin, ll.style);
    }

    wrefresh(mainWin);
}

// ── Message bar ───────────────────────────────────────────────────────────

void UI::message(const char* fmt, ...) {
    if (!msgWin) return;

    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    werase(msgWin);
    wattron(msgWin, COLOR_PAIR(CP_BORDER));
    box(msgWin, 0, 0);
    wattroff(msgWin, COLOR_PAIR(CP_BORDER));
    wattron(msgWin, COLOR_PAIR(CP_MSG));
    mvwprintw(msgWin, 1, 2, "%.*s", COLS - 6, buf);
    wattroff(msgWin, COLOR_PAIR(CP_MSG));
    wrefresh(msgWin);
}

// ── Stats panel ───────────────────────────────────────────────────────────

void UI::drawStats(const Player* player) {
    if (!statsWin || !player) return;

    werase(statsWin);
    wattron(statsWin, COLOR_PAIR(CP_BORDER));
    box(statsWin, 0, 0);
    wattroff(statsWin, COLOR_PAIR(CP_BORDER));

    int innerW = statsW - 2;
    int row    = 1;

    // ── Title ──
    wattron(statsWin, COLOR_PAIR(CP_STATS) | A_BOLD);
    mvwprintw(statsWin, row++, 1, "[ COMMANDER ]");
    wattroff(statsWin, COLOR_PAIR(CP_STATS) | A_BOLD);
    row++;  // blank line

    // ── HP numeric ──
    mvwprintw(statsWin, row++, 1, "HP: %d/%d", player->hp, player->maxHp);

    // ── HP bar ──
    {
        int barW   = innerW - 2;
        if (barW < 1) barW = 1;
        int filled = (player->maxHp > 0)
                        ? (player->hp * barW) / player->maxHp : 0;
        int hpPct  = (player->maxHp > 0)
                        ? (player->hp * 100) / player->maxHp : 0;
        int hpCP   = (hpPct > 60) ? CP_GOOD : (hpPct > 30) ? CP_ITEM : CP_ENEMY;

        mvwaddch(statsWin, row, 1, '[');
        for (int i = 0; i < barW; i++) {
            wattron(statsWin, COLOR_PAIR(i < filled ? hpCP : CP_NORMAL));
            waddch(statsWin, (i < filled) ? '#' : '.');
            wattroff(statsWin, COLOR_PAIR(i < filled ? hpCP : CP_NORMAL));
        }
        waddch(statsWin, ']');
        row += 2;   // bar row + blank line
    }

    // ── Weapon ──
    wattron(statsWin, A_BOLD);
    mvwprintw(statsWin, row++, 1, "WPN:");
    wattroff(statsWin, A_BOLD);
    if (player->equippedWeapon) {
        std::string wn = player->equippedWeapon->name;
        if ((int)wn.size() > innerW) wn = wn.substr(0, innerW);
        wattron(statsWin, COLOR_PAIR(CP_ITEM));
        mvwprintw(statsWin, row++, 1, "%s", wn.c_str());
        mvwprintw(statsWin, row++, 1, "%d dmg", player->equippedWeapon->damage);
        wattroff(statsWin, COLOR_PAIR(CP_ITEM));
    } else {
        mvwprintw(statsWin, row++, 1, "None");
        row++;
    }
    row++;  // blank line

    // ── Armor ──
    wattron(statsWin, A_BOLD);
    mvwprintw(statsWin, row++, 1, "ARM:");
    wattroff(statsWin, A_BOLD);
    if (player->equippedArmor) {
        std::string an = player->equippedArmor->name;
        if ((int)an.size() > innerW) an = an.substr(0, innerW);
        wattron(statsWin, COLOR_PAIR(CP_GOOD));
        mvwprintw(statsWin, row++, 1, "%s", an.c_str());
        mvwprintw(statsWin, row++, 1, "+%d def", player->equippedArmor->defensePower);
        wattroff(statsWin, COLOR_PAIR(CP_GOOD));
    } else {
        mvwprintw(statsWin, row, 1, "None");
    }

    wrefresh(statsWin);
}

// ── Room display ──────────────────────────────────────────────────────────

void UI::drawRoom(const Room* room) {
    log(STYLE_HEADER, "");
    log(STYLE_HEADER, "=== %s ===", room->name.c_str());
    log(STYLE_NORMAL, "%s", room->description.c_str());

    // Exits
    std::string exStr = "Exits:";
    bool any = false;
    if (room->exits[NORTH]) { exStr += " [N]"; any = true; }
    if (room->exits[SOUTH]) { exStr += " [S]"; any = true; }
    if (room->exits[EAST])  { exStr += " [E]"; any = true; }
    if (room->exits[WEST])  { exStr += " [W]"; any = true; }
    if (!any) exStr = "No exits. You are trapped.";
    log(STYLE_NORMAL, "%s", exStr.c_str());

    // Enemies
    if (!room->enemies.empty()) {
        std::string es = "Hostiles:";
        for (Enemy* e : room->enemies) es += "  " + e->name;
        log(STYLE_ENEMY, "%s", es.c_str());
    }

    // Items (with indices so [P] pickup makes sense)
    if (!room->items.empty()) {
        std::string is = "Items:";
        for (int i = 0; i < (int)room->items.size(); i++) {
            is += "  [" + std::to_string(i) + "] " + room->items[i]->name;
        }
        log(STYLE_ITEM, "%s", is.c_str());
    }
}

// ── Inventory display ─────────────────────────────────────────────────────

void UI::drawInventory(const Inventory* inv) {
    log(STYLE_HEADER, "--- INVENTORY (%d/%d) ---",
        (int)inv->items.size(), inv->maxCapacity);
    if (inv->items.empty()) {
        log(STYLE_NORMAL, "  (empty)");
        return;
    }
    for (int i = 0; i < (int)inv->items.size(); i++) {
        log(STYLE_ITEM, "  [%d] %s  -  %s",
            i, inv->items[i]->name.c_str(), inv->items[i]->description.c_str());
    }
}

// ── Hint bar ─────────────────────────────────────────────────────────────

void UI::redrawHint() {
    if (!hintWin) return;
    werase(hintWin);
    wattron(hintWin, COLOR_PAIR(CP_HINT) | A_BOLD);
    mvwprintw(hintWin, 0, 1,
        "[W/A/S/D] Move  [I] Inventory  [P] Pick up  [0-9] Use item  [Q] Quit");
    wattroff(hintWin, COLOR_PAIR(CP_HINT) | A_BOLD);
    wrefresh(hintWin);
}

// ── Input ─────────────────────────────────────────────────────────────────

int UI::getInput() {
    return wgetch(mainWin);
}
