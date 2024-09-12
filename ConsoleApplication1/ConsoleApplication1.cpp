#include <iostream>
#include <string>
#include <vector>
#include <fstream>  
#include <sstream>
using namespace std;

class Ticket {
private:
    string UserName;
    string Date;
    string FlightName;
    string Place;
    int Price;
    bool IsBooked;
    int ID;

public:
    // Constructor
    Ticket(int Price, const string& Date, const string& FlightName, const string& Place, int ID)
        : Price(Price), Date(Date), FlightName(FlightName), Place(Place), ID(ID), IsBooked(false), UserName("-") {}

    string GetUserName() const { return UserName; }
    bool GetIsBooked() const { return IsBooked; }
    string GetDate() const { return Date; }
    string GetFlightName() const { return FlightName; }
    string GetPlace() const { return Place; }
    int GetPrice() const { return Price; }
    int GetID() const { return ID; }

    void SetID(int newID) { ID = newID; }
    void SetUserName(const string& Name) {
        UserName = Name;
        IsBooked = true;
    }
    void DeleteBooking() {
        UserName = "-";
        IsBooked = false;
    }
    void DisplayTicketInfo() const {
        cout << "Ticket ID: " << ID << "\nPassenger: " << UserName << "\nFlight: " << FlightName
            << "\nSeat: " << Place << "\nPrice: $" << Price << "\nStatus: " << (IsBooked ? "Booked" : "Cancelled") << "\n";
    }
};

class User {
private:
    string UserName;
    std::vector<Ticket> tickets;

public:
    User(const string& UserName) : UserName(UserName) {}

    string GetUserName() const { return UserName; }

    // Return a non-const reference to allow modification were problem with it
    std::vector<Ticket>& GetUserTickets() { return tickets; }

    // Optionally keep the const version if you need read-only access elsewhere
    const std::vector<Ticket>& GetUserTickets() const { return tickets; }

    void AddTicket(const Ticket& ticket) {
        tickets.push_back(ticket);
    }
};



class Airplane {
private:
    std:: vector<Ticket> tickets;
    string Date;
    string FlightName;

public:
    // Constructor
    Airplane(const string& Date, const string& FlightName, int seatsPerRow)
        : Date(Date), FlightName(FlightName) {
        for (int i = 1; i <= seatsPerRow; ++i) {
            tickets.emplace_back(100, Date, FlightName, "Seat" + to_string(i), i);
        }
    }

    string GetDate() const { return Date; }
    string GetFlightName() const { return FlightName; }

    vector<Ticket> GetAvailableSeats() const {
        vector<Ticket> availableSeats;
        for (const auto& ticket : tickets) {
            if (!ticket.GetIsBooked()) {
                availableSeats.push_back(ticket);
            }
        }
        return availableSeats;
    }

    bool BookSeat(const string& UserName, const string& Place, int& ID, vector<Ticket>& BookedTickets, vector<User>& Users) {
        for (auto& ticket : tickets) {
            if (ticket.GetPlace() == Place && !ticket.GetIsBooked()) {
                ticket.SetUserName(UserName);
                ticket.SetID(ID);
                BookedTickets.push_back(ticket);
                bool userFound = false;

                for (auto& user : Users) {
                    if (user.GetUserName() == UserName) {
                        user.AddTicket(ticket);
                        userFound = true;
                        break;
                    }
                }

                if (!userFound) {
                    User newUser(UserName);
                    newUser.AddTicket(ticket);
                    Users.push_back(move(newUser));
                }

                cout << "Booking confirmed with ID " << ID << endl;
                return true;
            }
        }
        cout << "Place not available for booking.\n";
        return false;
    }

    bool CancelTicketByID(int ticketId) {
        for (auto& ticket : tickets) {
            if (ticket.GetID() == ticketId) {
                ticket.DeleteBooking();
                cout << "Booking canceled for ticket ID: " << ticketId << endl;
                return true;
            }
        }
        cout << "Ticket ID not found.\n";
        return false;
    }
};

class System {
private:
    vector<Airplane> flights;
    vector<User> users;
    vector<Ticket> bookedTickets;
    int newID = 11111;

public:
    // Load flights from file
    void LoadFlightsFromFile(const string& filePath) {
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
            flights.push_back(Airplane(date, flightName, seatsPerRow));
        }
        cout << "Flights loaded successfully from " << filePath << endl;
    }

    // Check flight availability
    void CheckFlightAvailability(const string& Date, const string& FlightName) {
        for (auto& flight : flights) {
            if (flight.GetDate() == Date && flight.GetFlightName() == FlightName) {
                auto availableSeats = flight.GetAvailableSeats();
                if (availableSeats.empty()) {
                    cout << "No available seats.\n";
                }
                else {
                    for (const auto& ticket : availableSeats) {
                        cout << "Seat: " << ticket.GetPlace() << ", Price: $" << ticket.GetPrice() << endl;
                    }
                }
                return;
            }
        }
        cout << "Flight not found.\n";
    }

    // Book a ticket
    void BookTicket(const string& UserName, const string& Date, const string& FlightName, const string& Seat) {
        for (auto& flight : flights) {
            if (flight.GetDate() == Date && flight.GetFlightName() == FlightName) {
                if (flight.BookSeat(UserName, Seat, newID, bookedTickets, users)) {
                    newID++;
                    return;
                }
            }
        }
        cout << "Flight or seat not found.\n";
    }

    // Cancel a ticket
    void CancelTicket(int ticketID) {
        for (auto& flight : flights) {
            if (flight.CancelTicketByID(ticketID)) {
                // Remove from user's booked tickets
                for (auto& user : users) {
                    auto& tickets = user.GetUserTickets();
                    tickets.erase(remove_if(tickets.begin(), tickets.end(),
                        [ticketID](const Ticket& ticket) { return ticket.GetID() == ticketID; }),
                        tickets.end());
                }
                return;
            }
        }
        cout << "Ticket ID not found.\n";
    }

    // View a ticket by ID
    void ViewTicketByID(int ticketID) {
        for (const auto& ticket : bookedTickets) {
            if (ticket.GetID() == ticketID) {
                ticket.DisplayTicketInfo();
                return;
            }
        }
        cout << "Ticket with given ID not found.\n";
    }

    // View tickets by username
    void ViewTicketsByName(const string& userName) {
        for (const auto& user : users) {
            if (user.GetUserName() == userName) {
                const auto& tickets = user.GetUserTickets();
                if (tickets.empty()) {
                    cout << "No tickets found for passenger: " << userName << endl;
                    return;
                }
                for (const auto& ticket : tickets) {
                    ticket.DisplayTicketInfo();
                }
                return;
            }
        }
        cout << "No user found with name: " << userName << endl;
    }
};

int main()
{
    cout << "Welcome to Airline Reservations System!\n";

    System airlineSystem;

    string filePath = "C:\\Users\\Кирило\\source\\repos\\Kyrylo0206\\OOP1\\DATA AIRLINES";
    airlineSystem.LoadFlightsFromFile(filePath);

    string command;
    while (true) {
        cout << "Choose the command:\n1) Check availability\n2) Book ticket\n3) Cancel ticket\n4) View ticket by ID\n5) View tickets by username\n6) Exit" << endl;
        cin >> command;

        if (command == "1") {
            string date, flightName;
            cout << "Enter flight date: ";
            cin >> date;
            cout << "Enter flight name: ";
            cin >> flightName;
            airlineSystem.CheckFlightAvailability(date, flightName);
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
            airlineSystem.BookTicket(userName, date, flightName, seat);
        }
        else if (command == "3") {
            int ticketID;
            cout << "Enter ticket ID to cancel: ";
            cin >> ticketID;
            airlineSystem.CancelTicket(ticketID);
        }
        else if (command == "4") {
            int ticketID;
            cout << "Enter ticket ID: ";
            cin >> ticketID;
            airlineSystem.ViewTicketByID(ticketID);
        }
        else if (command == "5") {
            string userName;
            cout << "Enter your name: ";
            cin >> userName;
            airlineSystem.ViewTicketsByName(userName);
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
