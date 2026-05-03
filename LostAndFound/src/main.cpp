#include <windows.h>
#include "Console.h"
#include "GameManager.h"

int main() {
    // UTF-8 + ANSI 이스케이프 코드 활성화
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Console::init();

    GameManager game;
    game.run();

    return 0;
}
