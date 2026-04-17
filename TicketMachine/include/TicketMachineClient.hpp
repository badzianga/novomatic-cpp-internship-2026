#ifndef TICKETMACHINE_TICKETMACHINECLIENT_HPP
#define TICKETMACHINE_TICKETMACHINECLIENT_HPP
#include <unordered_map>
#include "CashRegister.hpp"

class TicketMachineServer;

class TicketMachineClient {
public:
    explicit TicketMachineClient(int id, TicketMachineServer& server);
    void setInitialCashRegisterState(const std::unordered_map<Coin, int>& coins);
    void run();
private:
    static int sumOfCoins(const std::unordered_map<Coin, int>& coins);

    int id;
    CashRegister cashRegister;
    TicketMachineServer& server;
};

#endif //TICKETMACHINE_TICKETMACHINECLIENT_HPP
