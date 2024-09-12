#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Ticket {
private:
    string userName;
    string date;
    string flightName;
    string seat;
    int price;
    bool isBooked;
    int id;

public:
    // Constructor
    Ticket(int price, const string& date, const string& flightName, const string& seat, int id)
        : price(price), date(date), flightName(flightName), seat(seat), id(id), isBooked(false), userName("-") {}

    // Getters
    string getUserName() const { return userName; }
    bool getIsBooked() const { return isBooked; }
    string getDate() const { return date; }
    string getFlightName() const { return flightName; }
    string getSeat() const { return seat; }
    int getPrice() const { return price; }
    int getId() const { return id; }

    // Setters
    void setId(int newId) { id = newId; }
    void bookTicket(const string& name) {
        userName = name;
        isBooked = true;
    }
    void cancelBooking() {
        userName = "-";
        isBooked = false;
    }
    void displayTicketInfo() const {
        cout << "Ticket ID: " << id << "\nPassenger: " << userName << "\nFlight: " << flightName
            << "\nSeat: " << seat << "\nPrice: $" << price << "\nStatus: " << (isBooked ? "Booked" : "Cancelled") << "\n";
    }
};

class User {
private:
    string userName;
    vector<Ticket> tickets;

public:
    User(const string& userName) : userName(userName) {}

    string getUserName() const { return userName; }

    void addTicket(const Ticket& ticket) {
        tickets.push_back(ticket);
    }

    void removeTicketById(int ticketId) {
        tickets.erase(remove_if(tickets.begin(), tickets.end(),
            [ticketId](const Ticket& ticket) { return ticket.getId() == ticketId; }), tickets.end());
    }

    const vector<Ticket>& getTickets() const { return tickets; }
};

class Airplane {
private:
    vector<Ticket> tickets;
    string date;
    string flightName;

public:
    // Constructor
    Airplane(const string& date, const string& flightName, int seatsPerRow)
        : date(date), flightName(flightName) {
        for (int i = 1; i <= seatsPerRow; ++i) {
            tickets.emplace_back(100, date, flightName, "Seat" + to_string(i), 0); // ID will be set when booked
        }
    }

    string getDate() const { return date; }
    string getFlightName() const { return flightName; }

    vector<Ticket> getAvailableSeats() const {
        vector<Ticket> availableSeats;
        for (const auto& ticket : tickets) {
            if (!ticket.getIsBooked()) {
                availableSeats.push_back(ticket);
            }
        }
        return availableSeats;
    }

    Ticket* findTicketBySeat(const string& seat) {
        for (auto& ticket : tickets) {
            if (ticket.getSeat() == seat) {
                return &ticket;
            }
        }
        return nullptr;
    }

    bool bookSeat(const string& userName, const string& seat, int newId) {
        Ticket* ticket = findTicketBySeat(seat);
        if (ticket && !ticket->getIsBooked()) {
            ticket->bookTicket(userName);
            ticket->setId(newId);
            return true;
        }
        return false;
    }

    bool cancelTicketById(int ticketId) {
        for (auto& ticket : tickets) {
            if (ticket.getId() == ticketId) {
                if (ticket.getIsBooked()) {
                    ticket.cancelBooking();
                    return true;
                }
            }
        }
        return false;
    }

    Ticket* findBookedTicketById(int ticketId) {
        for (auto& ticket : tickets) {
            if (ticket.getId() == ticketId && ticket.getIsBooked()) {
                return &ticket;
            }
        }
        return nullptr;
    }
};

class System {
private:
    vector<Airplane> flights;
    vector<User> users;
    int newId = 11111;

public:
    void loadFlightsFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Error opening file: " << filePath << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string date, flightName;
            int seatsPerRow;
            ss >> date >> flightName >> seatsPerRow;
            flights.emplace_back(date, flightName, seatsPerRow);
        }
        cout << "Flights loaded successfully from " << filePath << endl;
    }

    void checkFlightAvailability(const string& date, const string& flightName) {
        for (const auto& flight : flights) {
            if (flight.getDate() == date && flight.getFlightName() == flightName) {
                auto availableSeats = flight.getAvailableSeats();
                if (availableSeats.empty()) {
                    cout << "No available seats.\n";
                }
                else {
                    for (const auto& ticket : availableSeats) {
                        cout << "Seat: " << ticket.getSeat() << ", Price: $" << ticket.getPrice() << endl;
                    }
                }
                return;
            }
        }
        cout << "Flight not found.\n";
    }

    void bookTicket(const string& userName, const string& date, const string& flightName, const string& seat) {
        for (auto& flight : flights) {
            if (flight.getDate() == date && flight.getFlightName() == flightName) {
                if (flight.bookSeat(userName, seat, newId)) {
                    Ticket* bookedTicket = flight.findTicketBySeat(seat);
                    if (bookedTicket) {
                        User* user = findOrCreateUser(userName);
                        user->addTicket(*bookedTicket);
                        cout << "Booking confirmed with ID " << newId << endl;
                        newId++;
                    }
                    return;
                }
                else {
                    cout << "Seat not available for booking.\n";
                    return;
                }
            }
        }
        cout << "Flight or seat not found.\n";
    }

    void cancelTicket(int ticketId) {
        for (auto& flight : flights) {
            if (flight.cancelTicketById(ticketId)) {
                // Remove ticket from user's booked tickets
                for (auto& user : users) {
                    user.removeTicketById(ticketId);
                }
                cout << "Booking canceled for ticket ID: " << ticketId << endl;
                return;
            }
        }
        cout << "Ticket ID not found.\n";
    }

 
    void viewTicketById(int ticketId) {
        for (const auto& flight : flights) {
            Ticket* ticket = flight.findBookedTicketById(ticketId);
            if (ticket) {
                ticket->displayTicketInfo();
                return;
            }
        }
        cout << "Ticket with given ID not found.\n";
    }

     void viewTicketsByName(const string& userName) {
        for (const auto& user : users) {
            if (user.getUserName() == userName) {
                const auto& tickets = user.getTickets();
                if (tickets.empty()) {
                    cout << "No tickets found for passenger: " << userName << endl;
                    return;
                }
                for (const auto& ticket : tickets) {
                    ticket.displayTicketInfo();
                }
                return;
            }
        }
        cout << "No user found with name: " << userName << endl;
    }

private:
    User* findOrCreateUser(const string& userName) {
        for (auto& user : users) {
            if (user.getUserName() == userName) {
                return &user;
            }
        }
        users.emplace_back(userName);
        return &users.back();
    }
};

int main() {
    cout << "Welcome to Airline Reservations System!\n";

    System airlineSystem;

    string filePath = "DATA AIRLINES.txt"; 
    airlineSystem.loadFlightsFromFile(filePath);

    string command;
    while (true) {
        cout << "\nChoose a command:\n1) Check availability\n2) Book ticket\n3) Cancel ticket\n"
            << "4) View ticket by ID\n5) View tickets by username\n6) Exit\n";
        cin >> command;

        if (command == "1") {
            string date, flightName;
            cout << "Enter flight date: ";
            cin >> date;
            cout << "Enter flight name: ";
            cin >> flightName;
            airlineSystem.checkFlightAvailability(date, flightName);
        }
        else if (command == "2") {
            string userName, date, flightName, seat;
            cout << "Enter your name: ";
            cin >> userName;
            cout << "Enter flight date: ";
            cin >> date;
            cout << "Enter flight name: ";
            cin >> flightName;
            cout << "Enter seat: ";
            cin >> seat;
            airlineSystem.bookTicket(userName, date, flightName, seat);
        }
        else if (command == "3") {
            int ticketId;
            cout << "Enter ticket ID to cancel: ";
            cin >> ticketId;
            airlineSystem.cancelTicket(ticketId);
        }
        else if (command == "4") {
            int ticketId;
            cout << "Enter ticket ID: ";
            cin >> ticketId;
            airlineSystem.viewTicketById(ticketId);
        }
        else if (command == "5") {
            string userName;
            cout << "Enter your name: ";
            cin >> userName;
            airlineSystem.viewTicketsByName(userName);
        }
        else if (command == "6") {
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid command. Please try again." << endl;
        }
    }

    return 0;
}
