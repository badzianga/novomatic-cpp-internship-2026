#include <ranges>
#include <gtest/gtest.h>
#include "CashRegister.hpp"

TEST(CashRegister, shouldGiveNoChangeWhenItIsNotNecessary) {
    CashRegister cashRegister;
    cashRegister.addCoins(Coin::ZL1, 2);

    std::map<Coin, int, std::greater<>> out;
    const bool result = cashRegister.giveChange(0, out);

    EXPECT_TRUE(result);
    EXPECT_TRUE(out.empty());
}

TEST(CashRegister, shouldGiveSimpleChange) {
    CashRegister cashRegister;
    cashRegister.addCoins(Coin::ZL2, 1);
    cashRegister.addCoins(Coin::ZL1, 5);
    cashRegister.addCoins(Coin::GR50, 2);

    std::map<Coin, int, std::greater<>> out;
    const bool result = cashRegister.giveChange(150, out);

    EXPECT_TRUE(result);
    EXPECT_EQ(out[Coin::ZL1], 1);
    EXPECT_EQ(out[Coin::GR50], 1);
}

TEST(CashRegister, shouldGiveMinAmountOfCoins) {
    CashRegister cashRegister;
    cashRegister.addCoins(Coin::GR50, 2);
    cashRegister.addCoins(Coin::GR20, 5);

    std::map<Coin, int, std::greater<>> out;
    const bool result = cashRegister.giveChange(60, out);

    EXPECT_TRUE(result);
    EXPECT_EQ(out[Coin::GR20], 3);
}

TEST(CashRegister, shouldRespectCoinLimits) {
    CashRegister cashRegister;
    cashRegister.addCoins(Coin::ZL2, 1);
    cashRegister.addCoins(Coin::ZL1, 5);

    std::map<Coin, int, std::greater<>> out;
    const bool result = cashRegister.giveChange(400, out);

    EXPECT_TRUE(result);
    EXPECT_EQ(out[Coin::ZL2], 1);
    EXPECT_EQ(out[Coin::ZL1], 2);
}

TEST(CashRegister, shouldFailIfDoesntHaveCoins) {
    CashRegister cashRegister;

    std::map<Coin, int, std::greater<>> out;
    const bool result = cashRegister.giveChange(100, out);

    EXPECT_FALSE(result);
    EXPECT_TRUE(out.empty());
}

TEST(CashRegister, shouldFailIfDoesntHaveEnoughCoins) {
    CashRegister cashRegister;
    cashRegister.addCoins(Coin::ZL2, 10);

    std::map<Coin, int, std::greater<>> out;
    const bool result = cashRegister.giveChange(220, out);

    EXPECT_FALSE(result);
    EXPECT_TRUE(out.empty());
}
