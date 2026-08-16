#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

// Detects Empty Line
string readNonEmptyLine(const string &prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty()) break;
        cout << "  -> This field cannot be empty. Please try again.\n";
    }
    return value;
}

// Reads a phone number and checks it only contains digits and has
// a reasonable length (7-15 digits covers most real phone numbers).
string readPhoneNumber(const string &prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        bool allDigits = !value.empty();
        for (char c : value) {
            if (!isdigit(static_cast<unsigned char>(c))) { allDigits = false; break; }
        }
        if (allDigits && value.length() >= 7 && value.length() <= 15) break;
        cout << "  -> Please enter a valid phone number (7-15 digits only).\n";
    }
    return value;
}

// Reads a positive double (used for distance in KM, weight in KG).
double readPositiveDouble(const string &prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cout << "  -> Please enter a valid positive number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

// Reads an integer menu choice within [minVal, maxVal].
int readMenuChoice(const string &prompt, int minVal, int maxVal) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= minVal && choice <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cout << "  -> Invalid choice. Please enter a number between "
             << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return choice;
}

// Reads "1" or "2" style choices for two named options (e.g. Bike/Scooter)
// and returns the chosen option's name as a string.
string readTwoWayChoice(const string &optionA, const string &optionB) {
    cout << "  1. " << optionA << "\n";
    cout << "  2. " << optionB << "\n";
    int choice = readMenuChoice("  Choose an option (1-2): ", 1, 2);
    return (choice == 1) ? optionA : optionB;
}

// =============================================================
//  CLASS: User
//  Stores the passenger/customer's basic details.
//  Demonstrates ENCAPSULATION - name and phone are private and
//  only reachable through public getters.
// =============================================================
class User {
private:
    string name;
    string phone;

public:
    // Constructor - initializes a User's details.
    User(const string &userName, const string &userPhone)
        : name(userName), phone(userPhone) {}

    // Getters (const member functions - they do not modify the object)
    string getName() const { return name; }
    string getPhone() const { return phone; }
};

// =============================================================
//  CLASS: Service (ABSTRACT BASE CLASS)
//  Every Gudau service (Ride, Food, Parcel, Repair) IS-A Service.
//  This is the root of the INHERITANCE hierarchy and demonstrates
//  ABSTRACTION: it defines *what* every service must be able to
//  do, without saying *how*.
// =============================================================
class Service {
protected:
    User *user;         // composition: a Service is requested BY a User
    string location;     // destination / customer location
    double fare;          // computed cost of the service

public:
    // Constructor - every service belongs to a user.
    Service(User *serviceUser) : user(serviceUser), location(""), fare(0.0) {}

    // Virtual destructor - required because we delete derived objects
    // through a Service* base pointer. (Note: pure virtual functions
    // must NOT be called from the base destructor, so cleanup here
    // only touches base-class data.)
    virtual ~Service() {}

    // Pure virtual functions -> make Service an ABSTRACT class.
    // Each derived service must ask its own specific questions.
    virtual void requestService() = 0;

    // Each derived service must know how to print its own summary.
    // Calling this through a Service* is a classic POLYMORPHISM example.
    virtual void displayDetails() const = 0;

    // Returns a human-readable name for the service (used for messages).
    virtual string getServiceName() const = 0;

    double getFare() const { return fare; }
};

// =============================================================
//  CLASS: RideBooking (derived from Service)
//  Demonstrates INHERITANCE + overriding (POLYMORPHISM).
// =============================================================
class RideBooking : public Service {
private:
    string vehicleType;   // Bike or Scooter
    double km;             // distance to travel

    static const int RATE_PER_KM = 16; // NPR per KM (from design)

public:
    RideBooking(User *u) : Service(u), vehicleType(""), km(0.0) {}

    // Destructor - demonstrates that each derived class can add its
    // own cleanup/closing behaviour on top of the base class.
    ~RideBooking() override {
        cout << "[System] Closing " << getServiceName() << " request.\n";
    }

    void requestService() override {
        cout << "\n----- Book a Ride -----\n";
        vehicleType = readTwoWayChoice("Bike", "Scooter");
        location = readNonEmptyLine("Where do you want to go? ");
        km = readPositiveDouble("Enter distance in KM: ");
        fare = km * RATE_PER_KM;
    }

    void displayDetails() const override {
        cout << "\n===== Ride Booking Confirmed =====\n";
        cout << "Passenger      : " << user->getName() << " (" << user->getPhone() << ")\n";
        cout << "Vehicle        : " << vehicleType << "\n";
        cout << "Destination    : " << location << " (" << km << " KM)\n";
        cout << "Fare           : NPR " << fixed << setprecision(2) << fare << "\n";
        cout << "===================================\n";
    }

    string getServiceName() const override { return "Ride Booking"; }
};

// =============================================================
//  CLASS: FoodDelivery (derived from Service)
// =============================================================
class FoodDelivery : public Service {
private:
    string vehicleType;   // Cycle or Scooter
    double km;

    static const int RATE_PER_KM = 9; // NPR per KM (from design)

public:
    FoodDelivery(User *u) : Service(u), vehicleType(""), km(0.0) {}

    ~FoodDelivery() override {
        cout << "[System] Closing " << getServiceName() << " request.\n";
    }

    void requestService() override {
        cout << "\n----- Food Delivery -----\n";
        vehicleType = readTwoWayChoice("Cycle", "Scooter");
        location = readNonEmptyLine("Where should the food be delivered? ");
        km = readPositiveDouble("Enter distance in KM: ");
        fare = km * RATE_PER_KM;
    }

    void displayDetails() const override {
        cout << "\n===== Food Delivery Confirmed =====\n";
        cout << "Customer       : " << user->getName() << " (" << user->getPhone() << ")\n";
        cout << "Delivery by    : " << vehicleType << "\n";
        cout << "Deliver to     : " << location << " (" << km << " KM)\n";
        cout << "Fare           : NPR " << fixed << setprecision(2) << fare << "\n";
        cout << "====================================\n";
    }

    string getServiceName() const override { return "Food Delivery"; }
};

// =============================================================
//  CLASS: ParcelDelivery (derived from Service)
//  Fare depends on weight brackets, and the assigned transport
//  changes automatically for heavy parcels - a good example of
//  business logic living inside the derived class.
// =============================================================
class ParcelDelivery : public Service {
private:
    double weight;        // in KG
    double km;
    string transport;     // Default: Scooter, or "4 Wheeler" if heavy

public:
    ParcelDelivery(User *u) : Service(u), weight(0.0), km(0.0), transport("Scooter") {}

    ~ParcelDelivery() override {
        cout << "[System] Closing " << getServiceName() << " request.\n";
    }

    void requestService() override {
        cout << "\n----- Parcel Delivery -----\n";
        weight = readPositiveDouble("How much does the parcel weigh (in KG)? ");
        location = readNonEmptyLine("Where should the parcel go? ");
        km = readPositiveDouble("Enter distance in KM: ");

        double ratePerKm;
        if (weight < 10) {
            ratePerKm = 12;
            transport = "Scooter";
        } else if (weight < 20) {
            ratePerKm = 14;
            transport = "Scooter";
        } else { // weight > 20 (heavy load)
            ratePerKm = 18;
            transport = "4 Wheeler";
            cout << "\nYour load is heavy so, 4 wheeler is coming to pick.\n";
        }

        fare = km * ratePerKm;
    }

    void displayDetails() const override {
        cout << "\n===== Parcel Delivery Confirmed =====\n";
        cout << "Sender         : " << user->getName() << " (" << user->getPhone() << ")\n";
        cout << "Transport      : " << transport << "\n";
        cout << "Deliver to     : " << location << " (" << km << " KM)\n";
        cout << "Parcel Weight  : " << weight << " KG\n";
        cout << "Fare           : NPR " << fixed << setprecision(2) << fare << "\n";
        cout << "======================================\n";
    }

    string getServiceName() const override { return "Parcel Delivery"; }
};

// =============================================================
//  CLASS: RepairService (derived from Service)
//  No fare is calculated for this service in the design - it
//  simply confirms that a repairman is on the way.
// =============================================================
class RepairService : public Service {
private:
    string repairItem;   // what needs to be repaired

public:
    RepairService(User *u) : Service(u), repairItem("") {}

    ~RepairService() override {
        cout << "[System] Closing " << getServiceName() << " request.\n";
    }

    void requestService() override {
        cout << "\n----- Repair Service -----\n";
        location = readNonEmptyLine("Enter your location: ");
        repairItem = readNonEmptyLine("What do you need repaired? ");
        fare = 0.0; // not specified in the design
    }

    void displayDetails() const override {
        cout << "\n===== Repair Service Confirmed =====\n";
        cout << "Customer       : " << user->getName() << " (" << user->getPhone() << ")\n";
        cout << "Repair Needed  : " << repairItem << "\n";
        cout << "Location       : " << location << "\n";
        cout << "Repairman is coming to your location.\n";
        cout << "=====================================\n";
    }

    string getServiceName() const override { return "Repair Service"; }
};

// =============================================================
//  Helper that runs any Service through a base-class pointer.
//  This one function works for ALL four services because of
//  POLYMORPHISM - it does not need to know which derived class
//  it was actually given.
// =============================================================
void runService(Service *service) {
    service->requestService();
    service->displayDetails();
}

// =============================================================
//  Prints the welcome banner and collects the user's details.
//  The details are only asked once, at the very start.
// =============================================================
User *registerUser() {
    cout << "========================================\n";
    cout << "              WELCOME TO GUDAU           \n";
    cout << "        Ride Sharing & Services          \n";
    cout << "========================================\n\n";

    string name = readNonEmptyLine("Enter your name: ");
    string phone = readPhoneNumber("Enter your phone number: ");

    cout << "\nWelcome, " << name << "! Let's get you started.\n";
    return new User(name, phone);
}

// =============================================================
//  Prints the Main Menu and returns the user's validated choice.
// =============================================================
int showMainMenu() {
    cout << "\n========== GUDAU MAIN MENU ==========\n";
    cout << "1. Book a Ride\n";
    cout << "2. Food Delivery\n";
    cout << "3. Parcel Delivery\n";
    cout << "4. Repair Service\n";
    cout << "0. Exit\n";
    cout << "=====================================\n";
    return readMenuChoice("Enter your choice: ", 0, 4);
}

// =============================================================
//  MAIN
//  System flow:
//    START -> USER DETAILS -> MAIN MENU -> SERVICE ->
//    SERVICE SUBMENU -> PERFORM OPERATION -> RETURN TO MAIN MENU
//    ... until the user chooses Exit.
// =============================================================
int main() {
    User *currentUser = registerUser();

    bool running = true;
    while (running) {
        int choice = showMainMenu();
        Service *service = nullptr;

        switch (choice) {
            case 1:
                service = new RideBooking(currentUser);
                break;
            case 2:
                service = new FoodDelivery(currentUser);
                break;
            case 3:
                service = new ParcelDelivery(currentUser);
                break;
            case 4:
                service = new RepairService(currentUser);
                break;
            case 0:
                running = false;
                break;
        }

        if (service != nullptr) {
            // Calling through a Service* base pointer -> POLYMORPHISM
            runService(service);
            delete service; // triggers the virtual destructor
        }
    }

    cout << "\n";
    cout << "       Thank you for using GUDAU!        \n";
    cout << "          Have a safe journey!           \n";
    cout << "";

    delete currentUser;
    return 0;
}