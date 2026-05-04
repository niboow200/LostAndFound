#include "FragileItem.h"
#include "Console.h"
#include <iostream>
using namespace std;

FragileItem::FragileItem(int id, string name, string description,
                         string owner_key, int max_examinations)
    : Item(id, move(name), move(description), move(owner_key))
    , examinations_left_(max_examinations) {}

void FragileItem::examine() {
    using namespace Console;

    if (broken_) {
        cout << "  " << paint("\xe2\x80\x9c완전히 망가져버렸다.\xe2\x80\x9d", A::RED) << "\n";
        return;
    }

    --examinations_left_;

    if (examinations_left_ == 0) {
        broken_      = true;
        name_        = "[파손됨] " + name_;
        description_ = "완전히 망가져버렸다.";

        cout << "  " << paint("[ 파손! ]", A::B_RED)
             << "  무리하게 다루다가 물건이 파손되었습니다!\n\n";
        cout << "  " << paint("\xe2\x80\x9c완전히 망가져버렸다.\xe2\x80\x9d", A::RED) << "\n";
    } else {
        cout << "  " << paint("[ 주의 ]", A::B_YELLOW)
             << "  파손까지 "
             << paint(to_string(examinations_left_) + "회", A::B_YELLOW)
             << " 남았습니다.\n\n";
        Item::examine();
    }
}
