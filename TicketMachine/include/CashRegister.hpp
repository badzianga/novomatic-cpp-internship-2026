#ifndef TICKETMACHINE_CASHREGISTER_HPP
#define TICKETMACHINE_CASHREGISTER_HPP
#include <map>
#include <unordered_map>
#include "Coin.hpp"

class CashRegister {
public:
    CashRegister();
    void addCoins(Coin coin, int amount);
    void addCoins(const std::unordered_map<Coin, int>& coins);
    void subtractCoins(const std::unordered_map<Coin, int>& coins);
    void subtractCoins(const std::map<Coin, int, std::greater<>>& coins);
    bool giveChange(int change, std::map<Coin, int, std::greater<>>& outCoins);
private:
    std::map<Coin, int, std::greater<>> coins;
};

#endif //TICKETMACHINE_CASHREGISTER_HPP
