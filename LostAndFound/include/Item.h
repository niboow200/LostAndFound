#pragma once
#include <string>
using std::string;

class Item {
public:
    Item(int id, string name, string description, string owner_key);
    virtual ~Item() = default;

    int           getId()          const;
    const string& getName()        const;
    const string& getDescription() const;
    const string& getOwnerKey()    const;

    virtual void examine();

protected:
    int    id_;
    string name_;
    string description_;
    string owner_key_;
};
