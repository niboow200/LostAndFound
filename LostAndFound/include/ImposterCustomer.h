#pragma once
#include "Customer.h"

// 사기꾼 손님 — 어떤 물건이든 자기 것이라 우기며 가져감
// 플레이어가 조사를 통해 이름 불일치를 발견해야 함
class ImposterCustomer : public Customer {
public:
    ImposterCustomer(std::string name, std::string owner_key, std::string dialogue = "");

    // owner_key 일치 여부와 무관하게 항상 true 반환
    bool verifyItem(const Item& item) const override;
};
