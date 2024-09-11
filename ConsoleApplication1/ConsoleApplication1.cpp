#include <iostream>
#include <string>
#include <vector>
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
    vector<Ticket> tickets;

public:
    User(const string& UserName) : UserName(UserName) {}

    string GetUserName() const { return UserName; }

    vector<Ticket>& GetUserTickets() { return tickets; }

    void AddTicket(const Ticket& ticket) {
        tickets.push_back(ticket);
    }
};

class Airplane {
private:
    vector<Ticket> tickets;
    string Date;
    string FlightName;

public:
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

                // Check if user exists and add ticket to the user
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
    void InitializeFlights() {
        flights.push_back(Airplane("2024-09-10", "Flight123", 6));
        // Additional flights can be added here
    }

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

    void CancelTicket(int ticketID) {
        for (auto& flight : flights) {
            if (flight.CancelTicketByID(ticketID)) {
                // Remove from user
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

    void ViewTicketByID(int ticketID) {
        for (const auto& ticket : bookedTickets) {
            if (ticket.GetID() == ticketID) {
                ticket.DisplayTicketInfo();
                return;
            }
        }
        cout << "Ticket with given ID not found.\n";
    }

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
	Airplane airplane("2024-09-10", "Flight123", 6);
	string command;
	while (true) {
		cout << "Choose the command:\n1)check\n2)book\n3)return\n4)viewID\n5)viewUsername\n6)viewDate\n7)viewFlightNo\n8)exit" << endl;
		cin >> command;
		if (command == "check") {
			airlineSystem.check();
		}
		else if (command == "book") {
			airlineSystem.book();
		}
		else if (command == "return") {
			airlineSystem.commandReturn();
		}
		else if (command == "viewID") {
			airlineSystem.viewID();
		}
		else if (command == "viewName") {
			airlineSystem.viewName();
		}
		else if (command == "viewDate") {
			airlineSystem.viewDate();
		}
		else if (command == "viewFlightNo") {
			airlineSystem.viewFlightNo();
		}
		else if (command == "exit") {
			break;
		}
		else {
			printf("You don't have this command\n");
		}
	}

	return 0;
}


