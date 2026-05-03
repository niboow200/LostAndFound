#include "Item.h"

Item::Item(int id, std::string name, std::string description)
    : id_(id)
    , name_(std::move(name))
    , description_(std::move(description)) {}

int                Item::getId()          const { return id_; }
const std::string& Item::getName()        const { return name_; }
const std::string& Item::getDescription() const { return description_; }
