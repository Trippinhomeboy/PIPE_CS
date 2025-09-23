#include <iostream>
#include <string>
#include <limits>
#include <fstream>

using namespace std;

struct Pipe {
    string name;
    double length = 0;
    double diameter = 0;
    bool isUnderRepair = false;
};

struct Cs {
    string name;
    int totalWorkshops = 0;
    int workingWorkshops = 0;
    double efficiency = 0;
};

template <typename T>
T getValidInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a valid number.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

void createPipe(Pipe& pipe) {
    cout << "\n=== Add New Pipe ===" << endl;
    cout << "Enter pipe name: ";
    getline(cin, pipe.name);

    pipe.length = getValidInput<double>("Enter pipe length (km): ");
    while (pipe.length <= 0) {
        cout << "Length must be positive. Try again.\n";
        pipe.length = getValidInput<double>("Enter pipe length (km): ");
    }

    pipe.diameter = getValidInput<double>("Enter pipe diameter (mm): ");
    while (pipe.diameter <= 0) {
        cout << "Diameter must be positive. Try again.\n";
        pipe.diameter = getValidInput<double>("Enter pipe diameter (mm): ");
    }

    pipe.isUnderRepair = false;
    cout << "Pipe '" << pipe.name << "' added successfully!\n" << endl;
}

void createCs(Cs& cs) {
    cout << "\n=== Add New Compressor Station ===" << endl;
    cout << "Enter station name: ";
    getline(cin, cs.name);

    cs.totalWorkshops = getValidInput<int>("Enter total workshops: ");
    while (cs.totalWorkshops <= 0) {
        cout << "Number must be positive. Try again.\n";
        cs.totalWorkshops = getValidInput<int>("Enter total workshops: ");
    }

    cs.workingWorkshops = getValidInput<int>("Enter working workshops: ");
    while (cs.workingWorkshops < 0 || cs.workingWorkshops > cs.totalWorkshops) {
        cout << "Working workshops cannot be negative or exceed total. Try again.\n";
        cs.workingWorkshops = getValidInput<int>("Enter working workshops: ");
    }

    cs.efficiency = getValidInput<double>("Enter efficiency (%): ");
    while (cs.efficiency < 0 || cs.efficiency > 100) {
        cout << "Efficiency must be between 0 and 100%. Try again.\n";
        cs.efficiency = getValidInput<double>("Enter efficiency (%): ");
    }
    cout << "Station '" << cs.name << "' added successfully!\n" << endl;
}

void displayAllObjects(const Pipe& pipe, const Cs& cs) {
    cout << "\n=== ALL OBJECTS ===" << endl;

    cout << "--- Pipe ---" << endl;
    if (pipe.name.empty()) {
        cout << "No pipe created." << endl;
    }
    else {
        cout << "Name: " << pipe.name << endl;
        cout << "Length: " << pipe.length << " km" << endl;
        cout << "Diameter: " << pipe.diameter << " mm" << endl;
        cout << "Under repair: " << (pipe.isUnderRepair ? "Yes" : "No") << endl;
    }

    cout << "\n--- Compressor Station ---" << endl;
    if (cs.name.empty()) {
        cout << "No station created." << endl;
    }
    else {
        cout << "Name: " << cs.name << endl;
        cout << "Total workshops: " << cs.totalWorkshops << endl;
        cout << "Working workshops: " << cs.workingWorkshops << endl;
        cout << "Efficiency: " << cs.efficiency << " %" << endl;
    }
    cout << "--------------------------------" << endl;
}

void editPipe(Pipe& pipe) {
    if (pipe.name.empty()) {
        cout << "Error: Create a pipe first." << endl;
        return;
    }
    cout << "\n=== Edit Pipe '" << pipe.name << "' ===" << endl;
    cout << "Current repair status: " << (pipe.isUnderRepair ? "Yes" : "No") << endl;
    cout << "1. Start repair" << endl;
    cout << "2. Stop repair" << endl;
    cout << "0. Cancel" << endl;

    int choice = getValidInput<int>("Select action: ");

    switch (choice) {
    case 1:
        pipe.isUnderRepair = true;
        cout << "Repair started." << endl;
        break;
    case 2:
        pipe.isUnderRepair = false;
        cout << "Repair stopped." << endl;
        break;
    case 0:
        cout << "Canceled." << endl;
        break;
    default:
        cout << "Invalid choice." << endl;
    }
}

void editCs(Cs& cs) {
    if (cs.name.empty()) {
        cout << "Error: Create a station first." << endl;
        return;
    }
    cout << "\n=== Edit Station '" << cs.name << "' ===" << endl;
    cout << "Working workshops: " << cs.workingWorkshops << " of " << cs.totalWorkshops << endl;
    cout << "1. Start workshop" << endl;
    cout << "2. Stop workshop" << endl;
    cout << "0. Cancel" << endl;

    int choice = getValidInput<int>("Select action: ");

    switch (choice) {
    case 1:
        if (cs.workingWorkshops < cs.totalWorkshops) {
            cs.workingWorkshops++;
            cout << "Workshop started. Now working: " << cs.workingWorkshops << " workshops." << endl;
        }
        else {
            cout << "Error: All workshops are already working." << endl;
        }
        break;
    case 2:
        if (cs.workingWorkshops > 0) {
            cs.workingWorkshops--;
            cout << "Workshop stopped. Now working: " << cs.workingWorkshops << " workshops." << endl;
        }
        else {
            cout << "Error: No working workshops to stop." << endl;
        }
        break;
    case 0:
        cout << "Canceled." << endl;
        break;
    default:
        cout << "Invalid choice." << endl;
    }
}

void saveToFile(const Pipe& pipe, const Cs& cs, const string& filename = "data.txt") {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file for writing." << endl;
        return;
    }

    if (!pipe.name.empty()) {
        file << "PIPE" << endl;
        file << pipe.name << endl;
        file << pipe.length << endl;
        file << pipe.diameter << endl;
        file << pipe.isUnderRepair << endl;
    }

    if (!cs.name.empty()) {
        file << "CS" << endl;
        file << cs.name << endl;
        file << cs.totalWorkshops << endl;
        file << cs.workingWorkshops << endl;
        file << cs.efficiency << endl;
    }

    file.close();
    cout << "Data saved to file '" << filename << "'." << endl;
}

void loadFromFile(Pipe& pipe, Cs& cs, const string& filename = "data.txt") {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file for reading." << endl;
        return;
    }

    string line;
    pipe.name = "";
    cs.name = "";

    while (getline(file, line)) {
        if (line == "PIPE") {
            getline(file, pipe.name);
            file >> pipe.length >> pipe.diameter >> pipe.isUnderRepair;
            file.ignore();
        }
        else if (line == "CS") {
            getline(file, cs.name);
            file >> cs.totalWorkshops >> cs.workingWorkshops >> cs.efficiency;
            file.ignore();
        }
    }

    file.close();
    cout << "Data loaded from file '" << filename << "'." << endl;
}

void displayMenu() {
    cout << "\n================================" << endl;
    cout << "             MENU" << endl;
    cout << "================================" << endl;
    cout << "1. Add pipe" << endl;
    cout << "2. Add station" << endl;
    cout << "3. View all objects" << endl;
    cout << "4. Edit pipe" << endl;
    cout << "5. Edit station" << endl;
    cout << "6. Save" << endl;
    cout << "7. Load" << endl;
    cout << "0. Exit" << endl;
    cout << "================================" << endl;
    cout << "Enter command number: ";
}

void processCommand(int command, Pipe& pipe, Cs& cs) {
    switch (command) {
    case 1:
        createPipe(pipe);
        break;
    case 2:
        createCs(cs);
        break;
    case 3:
        displayAllObjects(pipe, cs);
        break;
    case 4:
        editPipe(pipe);
        break;
    case 5:
        editCs(cs);
        break;
    case 6:
        saveToFile(pipe, cs);
        break;
    case 7:
        loadFromFile(pipe, cs);
        break;
    case 0:
        cout << "Exit program." << endl;
        exit(0);
    default:
        cout << "Command not recognized. Please select from menu." << endl;
    }
}

int main() {
    Pipe myPipe;
    Cs myCs;

    while (true) {
        displayMenu();
        int command = getValidInput<int>("");
        processCommand(command, myPipe, myCs);
    }
}