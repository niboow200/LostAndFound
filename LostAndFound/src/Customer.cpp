#include "Customer.h"
#include "Item.h"

Customer::Customer(std::string name, std::string claimed_item_name, std::string dialogue)
    : name_(std::move(name))
    , claimed_item_name_(std::move(claimed_item_name))
    , dialogue_(std::move(dialogue)) {}

const std::string& Customer::getName()            const { return name_; }
const std::string& Customer::getClaimedItemName() const { return claimed_item_name_; }

std::string Customer::getDialogueText() const {
    if (!dialogue_.empty()) return dialogue_;
    return "안녕하세요. 제 " + claimed_item_name_ + "을(를) 찾으러 왔습니다.";
}

std::string Customer::greet() const {
    return name_ + ": \"" + getDialogueText() + "\"";
}

bool Customer::verifyItem(const Item& item) const {
    // MVP: 아이템 이름 직접 비교
    // 확장 시: 단서(clue) 기반 다중 조건 검증으로 교체
    return item.getName() == claimed_item_name_;
}
