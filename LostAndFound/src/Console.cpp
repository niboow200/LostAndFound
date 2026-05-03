#include "Console.h"
#include <windows.h>
#include <iostream>
#include <algorithm>

void Console::init() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  mode = 0;
    if (GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void Console::clear() {
    std::cout << "\033[2J\033[1;1H";
}

// ── 문자열 래퍼 ───────────────────────────────────────────────────────────────

std::string Console::paint(const std::string& s, const char* color) {
    return std::string(color) + s + A::RST;
}

std::string Console::bold(const std::string& s) {
    return std::string(A::BOLD) + s + A::RST;
}

std::string Console::dim(const std::string& s) {
    return std::string(A::DIM) + s + A::RST;
}

// ── 평판 게이지 ───────────────────────────────────────────────────────────────

std::string Console::repBar(int value, int max_val, int bar_width) {
    value = std::clamp(value, 0, max_val);
    int filled = (bar_width * value) / max_val;
    int empty  = bar_width - filled;

    const char* c = (value > 60) ? A::B_GREEN
                  : (value > 30) ? A::B_YELLOW
                  :                A::B_RED;

    std::string bar(c);
    for (int i = 0; i < filled; ++i) bar += "\xe2\x96\x88";  // █
    bar += A::GRAY;
    for (int i = 0; i < empty;  ++i) bar += "\xe2\x96\x91";  // ░
    bar += A::RST;
    return bar;
}

// ── 레이아웃 출력 ─────────────────────────────────────────────────────────────

void Console::hRule(int width) {
    std::cout << A::GRAY << "  ";
    for (int i = 0; i < width; ++i) std::cout << "\xe2\x94\x81";  // ━
    std::cout << A::RST << "\n";
}

void Console::thinRule(int width) {
    std::cout << A::GRAY << "  ";
    for (int i = 0; i < width; ++i) std::cout << "\xe2\x94\x80";  // ─
    std::cout << A::RST << "\n";
}

void Console::printPhaseHeader(const std::string& icon,
                                const std::string& title,
                                const char*        color,
                                int                width) {
    std::cout << "\n";
    hRule(width);
    std::cout << "\n  " << icon << "  "
              << A::BOLD << color << title << A::RST << "\n\n";
    hRule(width);
    std::cout << "\n";
}

// ── 타이틀 카드 ───────────────────────────────────────────────────────────────

void Console::printTitleCard() {
    // 박스 문자 직접 임베드 (UTF-8 바이트 시퀀스)
    // ╔ \xe2\x95\x94   ═ \xe2\x95\x90   ╗ \xe2\x95\x97
    // ║ \xe2\x95\x91                     ╚ \xe2\x95\x9a   ╝ \xe2\x95\x9d

    auto dbl_h = [](int n) -> std::string {
        std::string s;
        for (int i = 0; i < n; ++i) s += "\xe2\x95\x90";  // ═
        return s;
    };

    const int W = 50;  // 박스 내부 너비 (터미널 열 기준)

    std::cout << "\n";
    // 상단 테두리
    std::cout << A::GRAY
              << "  \xe2\x95\x94" << dbl_h(W) << "\xe2\x95\x97\n"
              << A::RST;

    // 빈 줄
    std::cout << A::GRAY << "  \xe2\x95\x91" << A::RST
              << std::string(W, ' ')
              << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    // 제목 줄 1: 한국어 타이틀
    // "분실물 보관소 관리인" = 표시 너비 22 (한글 11자 × 2)
    // 좌여백 14 + 한글 22 + 우여백 14 = 50
    std::cout << A::GRAY << "  \xe2\x95\x91" << A::RST
              << std::string(15, ' ')
              << A::BOLD << A::B_YELLOW
              << "\xeb\xb6\x84\xec\x8b\xa4\xeb\xac\xbc \xeb\xb3\xb4\xea\xb4\x80\xec\x86\x8c \xea\xb4\x80\xeb\xa6\xac\xec\x9d\xb8"  // 분실물 보관소 관리인
              << A::RST
              << std::string(15, ' ')
              << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    // 제목 줄 2: 영어 서브타이틀
    // "Lost & Found Manager" = 20자  /  좌우 각 15칸
    std::cout << A::GRAY << "  \xe2\x95\x91" << A::RST
              << std::string(15, ' ')
              << A::DIM << A::CYAN
              << "Lost & Found Manager"
              << A::RST
              << std::string(15, ' ')
              << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    // 빈 줄
    std::cout << A::GRAY << "  \xe2\x95\x91" << A::RST
              << std::string(W, ' ')
              << A::GRAY << "\xe2\x95\x91\n" << A::RST;

    // 하단 테두리
    std::cout << A::GRAY
              << "  \xe2\x95\x9a" << dbl_h(W) << "\xe2\x95\x9d\n"
              << A::RST;
    std::cout << "\n";
}
