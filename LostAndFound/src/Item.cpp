#include "Item.h"
#include "Console.h"
#include <iostream>
using namespace std;

Item::Item(int id, string name, string description, string owner_key)
    : id_(id)
    , name_(move(name))
    , description_(move(description))
    , owner_key_(move(owner_key)) {}

int           Item::getId()          const { return id_; }
const string& Item::getName()        const { return name_; }
const string& Item::getDescription() const { return description_; }
const string& Item::getOwnerKey()    const { return owner_key_; }

void Item::examine() {
    cout << "  " << Console::A::CYAN
         << "\xe2\x80\x9c"
         << description_
         << "\xe2\x80\x9d"
         << Console::A::RST << "\n";
}
