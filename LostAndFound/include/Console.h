#pragma once
#include <string>
using std::string;

namespace Console {

    void init();
    void clear();

    namespace A {
        constexpr const char* RST       = "\033[0m";
        constexpr const char* BOLD      = "\033[1m";
        constexpr const char* DIM       = "\033[2m";
        constexpr const char* GRAY      = "\033[90m";
        constexpr const char* RED       = "\033[31m";
        constexpr const char* GREEN     = "\033[32m";
        constexpr const char* YELLOW    = "\033[33m";
        constexpr const char* CYAN      = "\033[36m";
        constexpr const char* MAGENTA   = "\033[35m";
        constexpr const char* B_RED     = "\033[91m";
        constexpr const char* B_GREEN   = "\033[92m";
        constexpr const char* B_YELLOW  = "\033[93m";
        constexpr const char* B_CYAN    = "\033[96m";
        constexpr const char* B_MAGENTA = "\033[95m";
        constexpr const char* B_WHITE   = "\033[97m";
    }

    string paint(const string& s, const char* color);
    string bold (const string& s);
    string dim  (const string& s);

    string repBar(int value, int max_val = 100, int bar_width = 20);

    void hRule    (int width = 52);
    void thinRule (int width = 52);

    void printPhaseHeader(const string& icon,
                          const string& title,
                          const char*   color,
                          int           width = 52);

    void printTitleCard();
}
