#include "Player.h"
#include "Console.h"
#include <algorithm>
#include <iostream>

Player::Player(int starting_money, int starting_reputation)
    : money_(starting_money), reputation_(starting_reputation) {}

int Player::getMoney()      const { return money_; }
int Player::getReputation() const { return reputation_; }

void Player::applyReward(int money_gain, int rep_gain) {
    money_      += money_gain;
    reputation_  = std::min(kMaxReputation, reputation_ + rep_gain);
}

void Player::applyPenalty(int money_loss, int rep_loss) {
    money_      -= money_loss;
    reputation_  = std::max(kMinReputation, reputation_ - rep_loss);
}

void Player::printStatus() const {
    using namespace Console;

    // 자금
    std::cout << "  "
              << A::BOLD << A::B_YELLOW << money_ << "\xec\x9b\x90" << A::RST  // 원
              << A::GRAY << "  |  " << A::RST;

    // 평판 게이지
    std::cout << A::GRAY << "\xed\x8f\x89\xed\x8c\x90 " << A::RST  // 평판 (UTF-8)
              << repBar(reputation_, kMaxReputation, 20)
              << "  "
              << A::BOLD << reputation_ << A::RST
              << A::GRAY << "/" << kMaxReputation << A::RST
              << "\n";
}
