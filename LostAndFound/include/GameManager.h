#pragma once
#include <vector>
#include <string>
#include "Item.h"
#include "Customer.h"
#include "Player.h"

enum class GamePhase {
    MORNING_INTAKE,     // 아침: 분실물 접수
    AFTERNOON_SERVICE,  // 낮:   손님 응대
    END_OF_DAY,         // 저녁: 하루 결산
    GAME_OVER
};

class GameManager {
public:
    GameManager();
    void run();

private:
    // --- 페이즈 진행 ---
    void morningPhase();
    void afternoonPhase();
    void endOfDayPhase();

    // --- 손님 한 명 응대 ---
    void handleCustomer(const Customer& customer, int index, int total);

    // --- 일차별 데이터 로딩 ---
    void loadDayData();

    // --- UI 헬퍼 ---
    void displayStorage()              const;
    int  getPlayerChoice(int max_index) const;

    // [확장 예정] 추리 / 미니게임
    // void examineItem     (const Item& item);
    // void questionCustomer(Customer& customer);
    // bool runMiniGame     (Customer& customer);

    std::vector<Item>     storage_;    // 보관소에 있는 물건들
    std::vector<Customer> customers_;  // 오늘 방문할 손님 큐
    Player                player_;
    GamePhase             current_phase_;
    int                   day_;
};
