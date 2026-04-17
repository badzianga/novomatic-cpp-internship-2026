#include "TicketMachineServer.hpp"

TicketMachineServer::TicketMachineServer(int initialTickets) {
    running = false;

    ticketTypes.emplace_back( "normalny", 350);
    ticketTypes.emplace_back("ulgowy", 170);
    ticketTypes.emplace_back("opiekuna", 220);
    for (int i = 0; i < initialTickets; ++i) {
        addNewTicket();
    }

    startThread();
}

TicketMachineServer::~TicketMachineServer() {
    stopThread();
}

const std::vector<std::pair<std::string, int>>& TicketMachineServer::getTicketTypes() const {
    return ticketTypes;
}

void TicketMachineServer::reserveTicket(int clientId, int ticketType) {
    std::lock_guard lock(mutex);

    if (reservations.contains(clientId)) {
        throw std::runtime_error("Client already has reservation");
    }

    if (ticketPool.empty()) {
        throw std::runtime_error("No tickets available");
    }

    const Ticket ticket = ticketPool.back();
    ticketPool.pop_back();

    const Reservation reservation{
        ticket,
        ticketType,
        std::chrono::system_clock::now()
    };

    reservations[clientId] = reservation;
}

void TicketMachineServer::cancelReservation(int clientId) {
    std::lock_guard lock(mutex);

    if (!reservations.contains(clientId)) return;

    ticketPool.push_back(reservations[clientId].ticket);
    reservations.erase(clientId);
}

Ticket TicketMachineServer::confirmPurchase(int clientId, const std::string& ownerName) {
    std::lock_guard lock(mutex);

    if (!reservations.contains(clientId)) {
        throw std::runtime_error("Transakcja anulowana: Sesja wygasla");
    }

    const auto& reservation = reservations[clientId];
    const auto&[ticketName, ticketPrice] = ticketTypes[reservation.ticketType];

    auto ticket = reservation.ticket;
    ticket.name = ticketName;
    ticket.price = ticketPrice;
    ticket.owner = ownerName;

    reservations.erase(clientId);
    return ticket;
}

void TicketMachineServer::timeoutCheck() {
    std::lock_guard lock(mutex);

    const auto now = std::chrono::system_clock::now();
    const auto timeout = std::chrono::seconds(60);

    for (auto it = reservations.begin(); it != reservations.end();) {
        if (now - it->second.timestamp > timeout) {
            ticketPool.push_back(it->second.ticket);
            it = reservations.erase(it);
        } else {
            ++it;
        }
    }
}

void TicketMachineServer::addNewTicket() {
    static int ticketId = 0;
    ticketPool.emplace_back(ticketId, 0, "");
}

bool TicketMachineServer::hasReservation(int clientId) {
    std::lock_guard lock(mutex);
    return reservations.contains(clientId);
}

void TicketMachineServer::startThread() {
    running = true;

    timeoutThread = std::thread([this]() {
        while (this->running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            this->timeoutCheck();
        }
    });
}

void TicketMachineServer::stopThread() {
    running = false;

    if (timeoutThread.joinable()) {
        timeoutThread.join();
    }
}
