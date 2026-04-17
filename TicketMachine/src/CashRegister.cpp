#include <limits>
#include <vector>
#include "CashRegister.hpp"

CashRegister::CashRegister() {
    coins[Coin::GR10] = 0;
    coins[Coin::GR20] = 0;
    coins[Coin::GR50] = 0;
    coins[Coin::ZL1] = 0;
    coins[Coin::ZL2] = 0;
    coins[Coin::ZL5] = 0;
}

void CashRegister::addCoins(Coin coin, int amount) {
    coins[coin] += amount;
}

void CashRegister::addCoins(const std::unordered_map<Coin, int>& inCoins) {
    for (auto [coin, amount] : inCoins) {
        this->coins[coin] += amount;
    }
}

void CashRegister::subtractCoins(const std::unordered_map<Coin, int>& outCoins) {
    for (auto [coin, amount] : outCoins) {
        this->coins[coin] -= amount;
    }
}

void CashRegister::subtractCoins(const std::map<Coin, int, std::greater<>>& coins) {
    for (auto [coin, amount] : coins) {
        this->coins[coin] -= amount;
    }
}

bool CashRegister::giveChange(int change, std::map<Coin, int, std::greater<>>& outCoins) {
    change /= 10;
    std::vector dp(change + 1, std::numeric_limits<int>::max());
    std::vector<std::map<Coin, int, std::greater<>>> used(change + 1);

    dp[0] = 0;

    for (auto& [coin, coinCount] : coins) {
        const int denom = toInt(coin) / 10;
        for (int i = change; i >= 0; --i) {
            if (dp[i] == std::numeric_limits<int>::max()) {
                continue;
            }

            for (int k = 1; k <= coinCount; ++k) {
                const int next = i + k * denom;
                if (next > change) {
                    break;
                }

                if (dp[next] > dp[i] + k) {
                    dp[next] = dp[i] + k;
                    used[next] = used[i];
                    used[next][coin] += k;
                }
            }
        }
    }

    if (dp[change] == std::numeric_limits<int>::max()) {
        return false;
    }

    outCoins = used[change];
    return true;
}
