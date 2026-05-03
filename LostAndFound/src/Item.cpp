#include "Item.h"

Item::Item(int id, std::string name, std::string description, std::string owner_key)
    : id_(id)
    , name_(std::move(name))
    , description_(std::move(description))
    , owner_key_(std::move(owner_key)) {}

int                Item::getId()          const { return id_; }
const std::string& Item::getName()        const { return name_; }
const std::string& Item::getDescription() const { return description_; }
const std::string& Item::getOwnerKey()    const { return owner_key_; }
