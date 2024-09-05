#include <iostream>
#include <string>
#include <vector>
#pragma once 
using namespace std;

class Airplane {
private:
    int total_seats = 204;
    vector <bool> availability_seat;
public:
	Airplane(int seats) : total_seats(seats), availability_seat(seats, true) {}
	//check is place free
	bool SeatAvailable(int seat_number) {
		if (seat_number < 0 || seat_number > total_seats) {
			cout << "Invalid seat number.\n";
			return false;
		return availability_seat  [seat_number - 1]; 
		}
	}
	bool Booking (int seat_number) {
		if (SeatAvailable(seat_number)) {
			availability_seat[seat_number - 1] = false;
			return true;
		}
		return false;
	}
	int getTotalSeats() const {
		return total_seats;
	}

};
class Ticket {
private:
    void booking_status(char check);

public:
   
};

int main()
{
	int c;

	string name;

	cout << "Welcome to Airline Reservations System!\n";
	cout << "Please enter your name: ";
	getline(cin, name);

	cout << "Choose a class: ";
	cin >> c;

}

