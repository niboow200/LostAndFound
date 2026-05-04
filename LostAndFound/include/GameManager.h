#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Item.h"
#include "Customer.h"
#include "Player.h"
using std::vector;
using std::unique_ptr;
using std::string;

enum class GamePhase {
    MORNING_INTAKE,
    AFTERNOON_SERVICE,
    END_OF_DAY,
    GAME_OVER
};

class GameManager {
public:
    GameManager();
    void run();

private:
    void morningPhase();
    void afternoonPhase();
    void endOfDayPhase();

    void handleCustomer   (Customer& customer, int index, int total);
    void handleExamineAction();

    void loadDayData();

    void displayStorage()               const;
    int  getPlayerChoice(int max_index) const;

    vector<unique_ptr<Item>>     storage_;
    vector<unique_ptr<Customer>> customers_;
    Player    player_;
    GamePhase current_phase_;
    int       day_;
};
