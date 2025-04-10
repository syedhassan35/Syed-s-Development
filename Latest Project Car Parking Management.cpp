#include <iostream>
#include <ctime>
#include <iomanip>
#include <set>
#include <regex>
using namespace std;

const double FEE_PER_HOUR_CAR = 10.0;
const double FEE_PER_HOUR_BIKE = 5.0;
const double FEE_PER_HOUR_TRUCK = 20.0;
const double LONG_TERM_DISCOUNT = 0.1;
const int MAX_PARKING_HOURS = 24;

struct Vehicle {
	string vehicleNumber;
	string vehicleType;
	time_t entryTime;
	int parkingSpot;
	string ownerPIN;
	Vehicle *next;

	Vehicle(string number, string type, int spot, string pin)
		: vehicleNumber(number), vehicleType(type), parkingSpot(spot), ownerPIN(pin), next(nullptr) {
		entryTime = time(0);
	}
};

struct Log {
	string vehicleNumber;
	string action;  // "entered" or "exited"
	time_t timestamp;
	Log *next;

	Log(string number, string act)
		: vehicleNumber(number), action(act), timestamp(time(0)), next(nullptr) {}
};

class ParkingLot {
	private:
		set<int> availableSpots;
		set<string> parkedVehicleNumbers;  // Set to store unique vehicle numbers
		Vehicle *parkedVehiclesHead;
		Log *logsHead;
		double totalRevenue;

	public:
		ParkingLot(int n) : parkedVehiclesHead(nullptr), logsHead(nullptr), totalRevenue(0) {
			for (int i = 1; i <= n; i++) {
				availableSpots.insert(i);
			}
		}

		bool isValidVehicleNumber(const string &number) {
			regex pattern("^[A-Za-z]{1,3}-[0-9]{1,4}$");
			return regex_match(number, pattern);
		}

		bool isValidVehicleType(const string &type) {
			return (type == "car" || type == "bike" || type == "truck");
		}

		bool isValidPIN(const string &pin) {
			regex pattern("^[0-9]{4}$");
			return regex_match(pin, pattern);
		}

		void displayAvailableSlots() {
			cout << "Available Parking Spots: ";
			for (int spot : availableSpots) {
				cout << spot << " ";
			}
			cout << "\n";
		}

		void displayLogs() {
			cout << "\nParking Lot Logs:\n";
			Log *temp = logsHead;
			while (temp != nullptr) {
				cout << "Vehicle " << temp->vehicleNumber << " " << temp->action << " at " << ctime(&(temp->timestamp));
				temp = temp->next;
			}
		}

		bool isVehicleNumberUnique(const string &vehicleNumber) {
			return parkedVehicleNumbers.find(vehicleNumber) == parkedVehicleNumbers.end();
		}

		void parkVehicle() {
			if (availableSpots.empty()) {
				cout << "Parking Full! No available spots.\n";
				return;
			}

			string vehicleNumber, vehicleType;
			cout << "Enter the vehicle number(ABC-123): ";
			cin >> vehicleNumber;
			if (!isValidVehicleNumber(vehicleNumber)) {
				cout << "Invalid format! Use: ABC-123\n";
				return;
			}

			// Check for duplicate vehicle number
			if (!isVehicleNumberUnique(vehicleNumber)) {
				cout << "Error: Vehicle number " << vehicleNumber << " is already parked!\n";
				return;
			}

			// Prompt for vehicle type and ensure valid input
			cout << "Enter the vehicle type (car/bike/truck): ";
			cin >> vehicleType;
			while (!isValidVehicleType(vehicleType)) {
				cout << "Invalid vehicle type! Please enter one of the following: car, bike, truck.\n";
				cout << "Enter the vehicle type (car/bike/truck): ";
				cin >> vehicleType;
			}

			displayAvailableSlots();
			int chosenSpot;
			cout << "Enter the slot number where you want to park: ";
			cin >> chosenSpot;

			if (availableSpots.find(chosenSpot) == availableSpots.end()) {
				cout << "Invalid slot selection! Choose from available spots.\n";
				return;
			}

			string ownerPIN;
			cout << "Set a 4-digit PIN for your vehicle: ";
			cin >> ownerPIN;
			if (!isValidPIN(ownerPIN)) {
				cout << "Invalid PIN! Must be 4 digits.\n";
				return;
			}

			Vehicle *newVehicle = new Vehicle(vehicleNumber, vehicleType, chosenSpot, ownerPIN);
			newVehicle->next = parkedVehiclesHead;
			parkedVehiclesHead = newVehicle;
			availableSpots.erase(chosenSpot);
			parkedVehicleNumbers.insert(vehicleNumber);  // Add vehicle number to the set of parked vehicles

			addLog(vehicleNumber, "entered");

			cout << "Vehicle " << vehicleNumber << " (" << vehicleType << ") parked at spot " << chosenSpot << ".\n";
		}

		void removeVehicle() {
			string vehicleNumber;
			cout << "Enter the vehicle number to remove: ";
			cin >> vehicleNumber;

			Vehicle *temp = parkedVehiclesHead;
			Vehicle *prev = nullptr;

			while (temp != nullptr) {
				if (temp->vehicleNumber == vehicleNumber) {
					break;
				}
				prev = temp;
				temp = temp->next;
			}

			if (temp == nullptr) {
				cout << "Vehicle not found!\n";
				return;
			}

			string enteredPIN;
			cout << "Enter the 4-digit PIN: ";
			cin >> enteredPIN;

			if (temp->ownerPIN != enteredPIN) {
				cout << "Invalid PIN! Access denied.\n";
				return;
			}

			time_t exitTime = time(0);
			double hoursParked = difftime(exitTime, temp->entryTime) / 3600.0;

			double feePerHour = (temp->vehicleType == "car") ? FEE_PER_HOUR_CAR :
			                    (temp->vehicleType == "bike") ? FEE_PER_HOUR_BIKE : FEE_PER_HOUR_TRUCK;

			double fee = hoursParked * feePerHour;
			if (hoursParked > 10) {
				fee -= fee * LONG_TERM_DISCOUNT;
			}

			cout << "\n=== Parking Fee Invoice ===\n";
			cout << "Vehicle Number: " << vehicleNumber << "\n";
			cout << "Hours Parked: " << fixed << setprecision(2) << hoursParked << " hours\n";
			cout << "Total Fee: $" << fixed << setprecision(2) << fee << "\n";

			double payment;
			cout << "Enter payment amount: $";
			cin >> payment;

			if (payment >= fee) {
				if (prev == nullptr) {
					parkedVehiclesHead = temp->next;
				} else {
					prev->next = temp->next;
				}

				availableSpots.insert(temp->parkingSpot);
				parkedVehicleNumbers.erase(vehicleNumber);  // Remove vehicle number from the set
				totalRevenue += fee;
				addLog(vehicleNumber, "exited");
				delete temp;
				cout << "Payment successful. Vehicle has left the parking lot.\n";
			} else {
				cout << "Insufficient payment!\n";
			}
		}

		void addLog(const string &vehicleNumber, const string &action) {
			Log *newLog = new Log(vehicleNumber, action);
			newLog->next = logsHead;
			logsHead = newLog;
		}

		void displayRevenue() {
			cout << "Total Revenue Collected: $" << fixed << setprecision(2) << totalRevenue << "\n";
		}

		void displayMenu() {
			cout << "\n--------------------------\n";
			cout << "  Parking Lot System\n";
			cout << "--------------------------\n";
			cout << "1. Park a vehicle\n";
			cout << "2. Remove a vehicle\n";
			cout << "3. Display Logs\n";
			cout << "4. Display Revenue\n";
			cout << "5. Show Available Slots\n";
			cout << "6. Exit\n";
			cout << "--------------------------\n";
		}
};

int main() {
	int totalSpots = 5;
	ParkingLot parkingLot(totalSpots);

	while (true) {
		parkingLot.displayMenu();
		cout << "Enter your choice: ";
		int choice;
		cin >> choice;

		switch (choice) {
			case 1:
				parkingLot.parkVehicle();
				break;
			case 2:
				parkingLot.removeVehicle();
				break;
			case 3:
				parkingLot.displayLogs();
				break;
			case 4:
				parkingLot.displayRevenue();
				break;
			case 5:
				parkingLot.displayAvailableSlots();
				break;
			case 6:
				cout << "Exiting Parking Lot System.\n";
				return 0;
			default:
				cout << "Invalid choice! Try again.\n";
		}

		cout << "\nPress Enter to continue...";
		cin.ignore();
		cin.get();
		system("clear || cls");
	}
}
