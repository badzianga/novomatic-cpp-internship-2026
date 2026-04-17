#ifndef TICKETMACHINE_COIN_HPP
#define TICKETMACHINE_COIN_HPP
#include <string>

enum class Coin {
    GR10 = 10,
    GR20 = 20,
    GR50 = 50,
    ZL1 = 100,
    ZL2 = 200,
    ZL5 = 500
};

int toInt(Coin coin);
std::string toString(Coin coin);

#endif //TICKETMACHINE_COIN_HPP
