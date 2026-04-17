#ifndef TICKETMACHINE_TICKET_HPP
#define TICKETMACHINE_TICKET_HPP
#include <string>

struct Ticket {
    int id;
    int price;
    std::string name;
    std::string owner;
};

#endif //TICKETMACHINE_TICKET_HPP
