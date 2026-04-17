#ifndef TICKETMACHINE_TICKETMACHINESERVER_HPP
#define TICKETMACHINE_TICKETMACHINESERVER_HPP
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include "Reservation.hpp"
#include "Ticket.hpp"

class TicketMachineServer {
public:
    explicit TicketMachineServer(int initialTickets);
    ~TicketMachineServer();

    [[nodiscard]] const std::vector<std::pair<std::string, int>>& getTicketTypes() const;
    void reserveTicket(int clientId, int ticketType);
    void cancelReservation(int clientId);
    Ticket confirmPurchase(int clientId, const std::string &ownerName);
    bool hasReservation(int clientId);
    void timeoutCheck();
private:
    void addNewTicket();
    void startThread();
    void stopThread();
    std::vector<std::pair<std::string, int>> ticketTypes;
    std::vector<Ticket> ticketPool;
    std::unordered_map<int, Reservation> reservations;  // key is clientId

    std::mutex mutex;
    std::thread timeoutThread;
    std::atomic<bool> running;
};

#endif //TICKETMACHINE_TICKETMACHINESERVER_HPP
