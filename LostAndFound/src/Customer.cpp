#include "Customer.h"
#include "Item.h"

Customer::Customer(std::string name, std::string owner_key, std::string dialogue)
    : name_(std::move(name))
    , owner_key_(std::move(owner_key))
    , dialogue_(std::move(dialogue)) {}

const std::string& Customer::getName()     const { return name_; }
const std::string& Customer::getOwnerKey() const { return owner_key_; }

std::string Customer::getDialogueText() const {
    if (!dialogue_.empty()) return dialogue_;
    return "안녕하세요. 제 물건을 찾으러 왔습니다.";
}

std::string Customer::greet() const {
    return name_ + ": \"" + getDialogueText() + "\"";
}

bool Customer::verifyItem(const Item& item) const {
    // MVP: owner_key 직접 비교
    // 확장 시: 단서(clue) 기반 다중 조건 검증으로 교체
    return item.getOwnerKey() == owner_key_;
}
