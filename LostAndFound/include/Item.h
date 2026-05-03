#pragma once
#include <string>
#include <vector>

class Item {
public:
    Item(int id, std::string name, std::string description = "");

    int                getId()          const;
    const std::string& getName()        const;
    const std::string& getDescription() const;

    // [확장 예정] 아이템 조사 시스템
    // std::string examine() const;
    // void        addClue(const std::string& clue);
    // const std::vector<std::string>& getClues() const;

private:
    int         id_;
    std::string name_;
    std::string description_;
    // std::vector<std::string> hidden_clues_;
};
