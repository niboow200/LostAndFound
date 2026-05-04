#pragma once
#include "Customer.h"

// VIP 손님 — 보상/페널티가 일반 손님의 2배
class VipCustomer : public Customer {
public:
    VipCustomer(std::string name, std::string owner_key, std::string dialogue = "");

    void processResult(bool isCorrect, Player& player, bool isDismissed = false) override;
};
