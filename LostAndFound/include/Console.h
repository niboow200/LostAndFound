#pragma once
#include <string>

// ANSI 색상 / 박스-드로잉 유틸리티
// Windows 콘솔과 Windows Terminal 모두 지원
namespace Console {

    // main() 최상단에서 한 번 호출 — ANSI 이스케이프 코드 활성화
    void init();

    // 화면 지우기 + 커서 좌상단 이동
    void clear();

    // ── ANSI 코드 상수 ────────────────────────────────────────────────────────
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

    // ── 문자열 래퍼 ───────────────────────────────────────────────────────────
    std::string paint(const std::string& s, const char* color);
    std::string bold (const std::string& s);
    std::string dim  (const std::string& s);

    // ── 평판 게이지 ───────────────────────────────────────────────────────────
    // 예: repBar(65, 100, 20)  →  "██████████████░░░░░░  65/100"
    std::string repBar(int value, int max_val = 100, int bar_width = 20);

    // ── 레이아웃 출력 ─────────────────────────────────────────────────────────
    void hRule    (int width = 52);   // 두꺼운 구분선 ━━━
    void thinRule (int width = 52);   // 얇은 구분선  ───

    // 페이즈 헤더: icon + 제목 + 색상
    void printPhaseHeader(const std::string& icon,
                          const std::string& title,
                          const char*        color,
                          int                width = 52);

    // 타이틀 카드 (게임 시작 화면)
    void printTitleCard();
}
