#ifndef TICKETMACHINE_RESERVATION_HPP
#define TICKETMACHINE_RESERVATION_HPP
#include <chrono>
#include "Ticket.hpp"

struct Reservation {
    Ticket ticket;
    int ticketType;
    std::chrono::system_clock::time_point timestamp;
};

#endif //TICKETMACHINE_RESERVATION_HPP
