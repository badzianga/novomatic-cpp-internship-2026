#include "Coin.hpp"

int toInt(Coin coin) {
    return static_cast<int>(coin);
}

std::string toString(Coin coin) {
    switch (coin) {
        case Coin::GR10: return "10gr";
        case Coin::GR20: return "20gr";
        case Coin::GR50: return "50gr";
        case Coin::ZL1: return "1zl";
        case Coin::ZL2: return "2zl";
        case Coin::ZL5: return "5zl";
        default: return "";
    }
}
