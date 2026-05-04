#include "Customer.h"
#include "Item.h"
#include "Player.h"
#include "Console.h"
#include <iostream>
using namespace std;

Customer::Customer(string name, string owner_key, string dialogue)
    : name_(move(name))
    , owner_key_(move(owner_key))
    , dialogue_(move(dialogue)) {}

const string& Customer::getName()     const { return name_; }
const string& Customer::getOwnerKey() const { return owner_key_; }

string Customer::getDialogueText() const {
    if (!dialogue_.empty()) return dialogue_;
    return "안녕하세요. 제 물건을 찾으러 왔습니다.";
}

string Customer::greet() const {
    return name_ + ": \"" + getDialogueText() + "\"";
}

bool Customer::verifyItem(const Item& item) const {
    return item.getOwnerKey() == owner_key_;
}

void Customer::processResult(bool isCorrect, Player& player, bool isDismissed) {
    using namespace Console;

    if (isDismissed) {
        player.applyPenalty(0, 10);
        cout << "  " << paint("   \xe2\x88\x92평판 10", A::RED) << "\n";
    } else if (isCorrect) {
        player.applyReward(50, 5);
        cout << "  " << paint("   +50\xec\x9b\x90  +\xed\x8f\x89\xed\x8c\x90 5", A::B_GREEN) << "\n";
    } else {
        player.applyPenalty(0, 15);
        cout << "  " << paint("   \xe2\x88\x92평판 15", A::RED) << "\n";
    }
}
