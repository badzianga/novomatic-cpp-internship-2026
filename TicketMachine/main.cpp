#include <iostream>

#include "TicketMachineClient.hpp"
#include "TicketMachineServer.hpp"

int main() {
    TicketMachineServer server(10);
    TicketMachineClient client(71830, server);

    std::unordered_map<Coin, int> initialCoins;
    initialCoins[Coin::GR10] = 10;
    initialCoins[Coin::GR20] = 10;
    initialCoins[Coin::GR50] = 10;
    initialCoins[Coin::ZL1] = 10;
    initialCoins[Coin::ZL2] = 10;
    initialCoins[Coin::ZL5] = 10;

    client.setInitialCashRegisterState(initialCoins);

    // normally it should be for (;;) on other thread, but I want to have access to the console
    for (int i = 0; i < 10; ++i) {
        try {
            client.run();
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
        }
    }

    return 0;
}
