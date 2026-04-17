#include <array>
#include <iomanip>
#include <iostream>
#include "Coin.hpp"
#include "UI.hpp"

int UI::getSelectedTicket(const std::vector<std::pair<std::string, int>>& ticketTypes) {
    std::cout << "Lista biletow:\n";
    int i = 1;
    for (const auto& [type, price] : ticketTypes) {
        std::cout << i++ << ". "
                  << type << ": "
                  << std::fixed << std::setprecision(2)
                  << static_cast<float>(price) / 100.f << "zl\n";
    }
    std::string input;
    for (;;) {
        std::cout << "Wybierz bilet (1-" << ticketTypes.size() << "): ";;
        try {
            std::getline(std::cin, input);
            const int selected = std::stoi(input);
            if (selected >= 1 && selected <= ticketTypes.size()) {
                return selected - 1;
            }
        } catch (...) {}
        std::cout << "Niepoprawny wybor\n";
    }
}

std::string UI::getUserData() {
    std::cout << "Podaj imie i nazwisko: ";
    std::string personData;
    std::getline(std::cin, personData);
    return personData;
}

std::unordered_map<Coin, int> UI::getInsertedCoins() {
    constexpr std::array coins = {
        Coin::GR10,
        Coin::GR20,
        Coin::GR50,
        Coin::ZL1,
        Coin::ZL2,
        Coin::ZL5,
    };

    std::unordered_map<Coin, int> insertedCoins(coins.size());

    for (Coin coin : coins) {
        std::string input;
        int amount;
        do {
            std::cout << "Ile monet " << toString(coin) << " wrzucasz?: ";
            try {
                std::getline(std::cin, input);
                amount = std::stoi(input);
            } catch (...) {
                amount = -1;
            }
        } while (amount < 0);
        insertedCoins[coin] = amount;
    }
    return insertedCoins;
}

bool UI::cancel() {
    std::cout << "Zanim przejdziesz dalej - czy chcesz anulowac?\n"
                 "Operacji nie mozna cofnac az do jej ukonczenia (X jesli tak): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    return confirm == "X" || confirm == "x";
}

void UI::displayChange(const std::map<Coin, int, std::greater<>>& coins) {
    std::cout << "Wydana reszta:\n";
    for (auto [coin, amount] : coins) {
        std::cout << amount << " x " << toString(coin) << '\n';
    }
}

void UI::displaySuccess(const Ticket& ticket) {
    std::cout << "Bilet zakupiony pomyslnie:\n"
                 "ID: " << ticket.id << "\n"
                 "Nazwa: " << ticket.name << "\n"
                 "Cena: " << static_cast<float>(ticket.price) / 100.f << "zl\n"
                 "Wlasciciel: " << ticket.owner << "\n\n";
}
