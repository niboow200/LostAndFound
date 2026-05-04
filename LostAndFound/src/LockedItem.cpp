#include "LockedItem.h"
#include "Console.h"
#include <iostream>
#include <string>
using namespace std;

LockedItem::LockedItem(int id, string name, string description,
                       string owner_key, string password)
    : Item(id, move(name), move(description), move(owner_key))
    , password_(move(password)) {}

void LockedItem::examine() {
    using namespace Console;

    if (unlocked_) {
        Item::examine();
        return;
    }

    cout << "  " << paint("[ 잠김 ]", A::B_RED)
         << "  이 물건은 잠겨있습니다.\n";
    cout << "  비밀번호를 입력하세요  > ";

    string input;
    getline(cin, input);

    if (input == password_) {
        unlocked_ = true;
        cout << "\n  " << paint("\xe2\x9c\x94  잠금 해제!", A::B_GREEN) << "\n\n";
        Item::examine();
    } else {
        cout << "\n  " << paint("\xe2\x9c\x98  잘못된 비밀번호입니다.", A::B_RED) << "\n";
    }
}
