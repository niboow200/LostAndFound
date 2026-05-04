#pragma once
#include "Item.h"
#include <string>

class LockedItem : public Item {
public:
    LockedItem(int id, string name, string description,
               string owner_key, string password);

    void examine() override;

private:
    string password_;
    bool   unlocked_ = false;
};
