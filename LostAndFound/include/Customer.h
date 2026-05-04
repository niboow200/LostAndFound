#pragma once
#include <string>
using std::string;

class Item;
class Player;

class Customer {
public:
    Customer(string name, string owner_key, string dialogue = "");
    virtual ~Customer() = default;

    const string& getName()     const;
    const string& getOwnerKey() const;

    virtual string greet()                     const;
    virtual string getDialogueText()            const;
    virtual bool   verifyItem(const Item& item) const;

    virtual void processResult(bool isCorrect, Player& player, bool isDismissed = false);

protected:
    string name_;
    string owner_key_;
    string dialogue_;
};
