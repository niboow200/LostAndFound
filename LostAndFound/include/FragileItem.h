#pragma once
#include "Item.h"

// 파손 주의 물건 — 조사 횟수 초과 시 영구 파손
class FragileItem : public Item {
public:
    FragileItem(int id, std::string name, std::string description,
                std::string owner_key, int max_examinations = 3);

    void examine() override;

private:
    int  examinations_left_;
    bool broken_ = false;
};
