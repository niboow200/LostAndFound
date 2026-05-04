#include "VipCustomer.h"
#include "Player.h"
#include "Console.h"
#include <iostream>
using namespace std;

VipCustomer::VipCustomer(string name, string owner_key, string dialogue)
    : Customer(move(name), move(owner_key), move(dialogue)) {}

void VipCustomer::processResult(bool isCorrect, Player& player, bool isDismissed) {
    using namespace Console;

    if (isDismissed) {
        player.applyPenalty(0, 20);
        cout << "  " << paint("   \xe2\x88\x92평판 20  ★ VIP 페널티", A::B_RED) << "\n";
    } else if (isCorrect) {
        player.applyReward(100, 10);
        cout << "  " << paint("   +100원  +평판 10  ★ VIP 보상", A::B_GREEN) << "\n";
    } else {
        player.applyPenalty(0, 30);
        cout << "  " << paint("   \xe2\x88\x92평판 30  ★ VIP 페널티", A::B_RED) << "\n";
    }
}
