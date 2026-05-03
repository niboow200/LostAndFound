#pragma once
#include <string>

class Item;  // forward declaration — Customer는 Item을 소유하지 않음

class Customer {
public:
    Customer(std::string name, std::string claimed_item_name, std::string dialogue = "");

    const std::string& getName()            const;
    const std::string& getClaimedItemName() const;

    std::string greet()           const;  // "이름: \"대사\"" 형식 전체 반환
    std::string getDialogueText() const;  // 대사 텍스트만 반환 (UI 분리용)
    bool        verifyItem(const Item& item) const;

    // [확장 예정] 질문 / 미니게임 시스템
    // std::string answerQuestion(const std::string& topic) const;
    // bool        isDifficult()                             const;
    // bool        runMiniGame();

private:
    std::string name_;
    std::string claimed_item_name_;
    std::string dialogue_;

    // [확장 예정]
    // bool is_difficult_ = false;
    // std::map<std::string, std::string> qa_table_;
};
