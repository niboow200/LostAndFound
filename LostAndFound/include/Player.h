#pragma once

class Player {
public:
    Player(int starting_money = 100, int starting_reputation = 50);

    int getMoney()      const;
    int getReputation() const;

    void applyReward (int money_gain, int rep_gain);
    void applyPenalty(int money_loss, int rep_loss);

    void printStatus() const;

private:
    int money_;
    int reputation_;

    static constexpr int kMaxReputation = 100;
    static constexpr int kMinReputation = 0;
};
