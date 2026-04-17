#include <array>
#include "TicketMachineClient.hpp"
#include "TicketMachineServer.hpp"
#include "UI.hpp"

TicketMachineClient::TicketMachineClient(int id, TicketMachineServer& server)
    : id(id), server(server) {}

void TicketMachineClient::setInitialCashRegisterState(const std::unordered_map<Coin, int>& coins) {
    // by default, cashRegister has all coin keys initialized, so add coins only defined in arg
    for (auto& [coin, amount] : coins) {
        cashRegister.addCoins(coin, amount);
    }
}

void TicketMachineClient::run() {
    constexpr std::array ticketPrices = { 350, 170, 220 };

    const int ticketType = UI::getSelectedTicket(server.getTicketTypes());
    server.reserveTicket(id, ticketType);

    const std::string userData = UI::getUserData();

    if (!server.hasReservation(id)) {
        throw std::runtime_error("Transakcja anulowana: Sesja wygasla");
    }

    if (UI::cancel()) {
        server.cancelReservation(id);
        throw std::runtime_error("Anulowano zakup");
    }

    if (!server.hasReservation(id)) {
        throw std::runtime_error("Transakcja anulowana: Sesja wygasla");
    }

    const std::unordered_map<Coin, int> insertedCoins = UI::getInsertedCoins();
    cashRegister.addCoins(insertedCoins);

    const int sum = sumOfCoins(insertedCoins);
    if (sum == 0) {
        server.cancelReservation(id);
        throw std::runtime_error("Niewystaczajaca kwota");
    }
    const int price = ticketPrices[ticketType];
    const int change = sum - price;

    std::map<Coin, int, std::greater<>> outCoins;
    if (!cashRegister.giveChange(change, outCoins)) {
        server.cancelReservation(id);
        cashRegister.subtractCoins(insertedCoins);  // remove inserted coins
        throw std::runtime_error("Transakcja odrzucona: Brak mozliwosci wydania reszty");
    }
    const Ticket ticket = server.confirmPurchase(id, userData);
    cashRegister.subtractCoins(outCoins);  // remove change coins
    UI::displayChange(outCoins);
    UI::displaySuccess(ticket);
}

int TicketMachineClient::sumOfCoins(const std::unordered_map<Coin, int>& coins) {
    int sum = 0;
    for (auto& [coin, amount] : coins) {
        sum += toInt(coin) * amount;
    }
    return sum;
}
