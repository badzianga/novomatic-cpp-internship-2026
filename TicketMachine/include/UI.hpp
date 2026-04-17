#ifndef TICKETMACHINE_UI_HPP
#define TICKETMACHINE_UI_HPP
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "Coin.hpp"
#include "Ticket.hpp"

class UI {
public:
    [[nodiscard]] static int getSelectedTicket(const std::vector<std::pair<std::string, int>>& ticketTypes);
    [[nodiscard]] static std::string getUserData();
    [[nodiscard]] static std::unordered_map<Coin, int> getInsertedCoins();
    [[nodiscard]] static bool cancel();
    static void displayChange(const std::map<Coin, int, std::greater<>>& coins);
    static void displaySuccess(const Ticket &ticket);
};

#endif //TICKETMACHINE_UI_HPP
