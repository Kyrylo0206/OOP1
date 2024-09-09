#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#pragma once 
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
	Ticket(const& int Price, const& string UserName, const& string Date, const& string FlightName, const& string Place, const& bool IsBooked, const& int ID) {
		: UserName(UserName), Date(Date), Place(Place), FlightName(FlightName), Price(Price), IsBooked(true){}
		
		string GetUserName() const {
			return UserName;
		}

		bool GetIsBooked() const {
			return IsBooked;
		}

		string GetDate() const {
			return Date;
		}

		string GetFlightName() const {
			return FlightName;
		}

		string GetPlace() const {
			return Place;
		}

		int GetPrice() const {
			return Price;
		}

		int GetID() const {
			return ID;
		}

		void SetID(int newID) {
			ID = newID;
		}

		void SetUserName(const string & Name) {
			UserName = Name;
			IsBooked = false;
		}

		void DeleteBooking() {
			UserName = "-";
			IsBooked = true;
		}

		void GetinfoTicket()const {
			cout << "Ticket ID: " << ID << "\nPassenger: " << UserName << "\nFlight: " << FlightName << "\nSeat: " << Place
				<< "\nPrice: $" << Price << "\nStatus: " << (IsBooked ? "Booked" : "Cancelled") << "\n";
		}

		void SetID(int newID) { 
			ID = newID; 
		}
		void SetUserName(const string& name) { 
			UserName = name; IsBooked = false; 
		}
		void DeleteBooking() { 
			UserName = "-"; IsBooked = false; 
		}
	};

class Airplane {
private:
	vector<Ticket> tickets;
	string Date;
	string flightName;
	int TotalSize;
	int seatsPerRow;

public:
	Airplane(const string& date, const string& flightNo, int seats) : Date(date), FlightNo(flightNo), seatsPerRow(seats) {}
	//check is place free 
	Ticket<vector> SeatAvailable() const {
		Ticket<vector> SeatAvailable;
		for (const auto& ticket : tickets) {
			if (ticket.GetIsBooked() == true && ticket.GetUserName() == "-") {
				SeatAvailable.push_back(ticket);
			}
		}
		return SeatAvailable;
	}
	bool Booking (int seats) {
		if (SeatAvailable(seats)) {
			availability_seat[seats - 1] = false;
			return true;
		}
		return false;
	}
	int getTotalSeats() const {
		return total_seats;
	}
};

class User {
	string UserName;
	vector<Ticket> tickets;

public:
	User(const string& UserName) : UserName(UserName) {}

	string GetUserName() const {
		return UserName;
	}

	vector<Ticket>& UserTickets() {
		return tickets;
	}

	void addTicket(const Ticket& ticket) {
		tickets.push_back(ticket);
	}
};

int main()
{
	cout << "Welcome to Airline Reservations System!\n";
	AirlineSystem airlineSystem("/C:/Users/Кирило/source/repos/Kyrylo0206/OOP1/DATA AIRLINES");
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

}

