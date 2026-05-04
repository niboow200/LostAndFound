#include "ImposterCustomer.h"
#include "Item.h"

ImposterCustomer::ImposterCustomer(std::string name, std::string owner_key, std::string dialogue)
    : Customer(std::move(name), std::move(owner_key), std::move(dialogue)) {}

bool ImposterCustomer::verifyItem(const Item& /*item*/) const {
    // 어떤 물건이든 "맞다"고 우기며 가져감
    // 플레이어가 조사를 통해 이름 불일치를 발견해야 함
    return true;
}
