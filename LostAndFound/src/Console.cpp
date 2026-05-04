#include "Console.h"
#include <windows.h>
#include <iostream>
#include <algorithm>
using namespace std;

void Console::init() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  mode = 0;
    if (GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void Console::clear() {
    cout << "\033[2J\033[1;1H";
}

string Console::paint(const string& s, const char* color) {
    return string(color) + s + A::RST;
}

string Console::bold(const string& s) {
    return string(A::BOLD) + s + A::RST;
}

string Console::dim(const string& s) {
    return string(A::DIM) + s + A::RST;
}

string Console::repBar(int value, int max_val, int bar_width) {
    value = clamp(value, 0, max_val);
    int filled = (bar_width * value) / max_val;
    int empty  = bar_width - filled;

    const char* c = (value > 60) ? A::B_GREEN
                  : (value > 30) ? A::B_YELLOW
                  :                A::B_RED;

    string bar(c);
    for (int i = 0; i < filled; ++i) bar += "\xe2\x96\x88";  // █
    bar += A::GRAY;
    for (int i = 0; i < empty;  ++i) bar += "\xe2\x96\x91";  // ░
    bar += A::RST;
    return bar;
}

void Console::hRule(int width) {
    cout << A::GRAY << "  ";
    for (int i = 0; i < width; ++i) cout << "\xe2\x94\x81";  // ━
    cout << A::RST << "\n";
}

void Console::thinRule(int width) {
    cout << A::GRAY << "  ";
    for (int i = 0; i < width; ++i) cout << "\xe2\x94\x80";  // ─
    cout << A::RST << "\n";
}

void Console::printPhaseHeader(const string& icon,
                                const string& title,
                                const char*   color,
                                int           width) {
    cout << "\n";
    hRule(width);
    cout << "\n  " << icon << "  "
         << A::BOLD << color << title << A::RST << "\n\n";
    hRule(width);
    cout << "\n";
}

void Console::printTitleCard() {
    auto dbl_h = [](int n) -> string {
        string s;
        for (int i = 0; i < n; ++i) s += "\xe2\x95\x90";  // ═
        return s;
    };

    const int W = 50;

    cout << "\n";
    cout << A::GRAY
         << "  \xe2\x95\x94" << dbl_h(W) << "\xe2\x95\x97\n"
         << A::RST;

    cout << A::GRAY << "  \xe2\x95\x91" << A::RST
         << string(W, ' ')
         << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    cout << A::GRAY << "  \xe2\x95\x91" << A::RST
         << string(15, ' ')
         << A::BOLD << A::B_YELLOW
         << "\xeb\xb6\x84\xec\x8b\xa4\xeb\xac\xbc \xeb\xb3\xb4\xea\xb4\x80\xec\x86\x8c \xea\xb4\x80\xeb\xa6\xac\xec\x9d\xb8"
         << A::RST
         << string(15, ' ')
         << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    cout << A::GRAY << "  \xe2\x95\x91" << A::RST
         << string(15, ' ')
         << A::DIM << A::CYAN
         << "Lost & Found Manager"
         << A::RST
         << string(15, ' ')
         << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    cout << A::GRAY << "  \xe2\x95\x91" << A::RST
         << string(W, ' ')
         << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    cout << A::GRAY
         << "  \xe2\x95\x9a" << dbl_h(W) << "\xe2\x95\x9d\n"
         << A::RST;
    cout << "\n";
}
