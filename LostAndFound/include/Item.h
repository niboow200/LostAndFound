#pragma once
#include <string>

class Item {
public:
    // name        : 보관소에 표시되는 물리적 설명 ("갈색 가죽 지갑")
    // description : 조사(Examine) 시 보여줄 텍스트
    // owner_key   : 소유자 식별 키 — 플레이어에게 비공개, 매칭에만 사용
    Item(int id, std::string name, std::string description, std::string owner_key);

    int                getId()          const;
    const std::string& getName()        const;  // 표시 이름
    const std::string& getDescription() const;  // 조사 텍스트
    const std::string& getOwnerKey()    const;  // 소유자 키 (비공개)

    // [확장 예정] 단서 목록, 다중 조사 단계
    // void        addClue(const std::string& clue);
    // const std::vector<std::string>& getClues() const;

private:
    int         id_;
    std::string name_;
    std::string description_;
    std::string owner_key_;
    // std::vector<std::string> clues_;
};
