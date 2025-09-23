#include <iostream>
#include <string>
#include <limits>

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
            cout << "Ошибка: Введите корректное число.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

void createPipe(Pipe& pipe) {
    cout << "\n=== Добавление новой трубы ===" << endl;
    cout << "Введите название трубы: ";
    getline(cin, pipe.name);

    pipe.length = getValidInput<double>("Введите длину трубы (км): ");
    while (pipe.length <= 0) {
        cout << "Длина должна быть положительной. Попробуйте снова.\n";
        pipe.length = getValidInput<double>("Введите длину трубы (км): ");
    }

    pipe.diameter = getValidInput<double>("Введите диаметр трубы (мм): ");
    while (pipe.diameter <= 0) {
        cout << "Диаметр должен быть положительным. Попробуйте снова.\n";
        pipe.diameter = getValidInput<double>("Введите диаметр трубы (мм): ");
    }

    pipe.isUnderRepair = false;
    cout << "Труба '" << pipe.name << "' успешно добавлена!\n" << endl;
}

void displayMenu() {
    cout << "\n================================" << endl;
    cout << "             МЕНЮ" << endl;
    cout << "================================" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать КС" << endl;
    cout << "6. Сохранить" << endl;
    cout << "7. Загрузить" << endl;
    cout << "0. Выход" << endl;
    cout << "================================" << endl;
    cout << "Введите номер команды: ";
}

int main() {
    Pipe myPipe;
    Cs myCs;
    int command;

    while (true) {
        displayMenu();
        command = getValidInput<int>("");

        switch (command) {
        case 1:
            createPipe(myPipe);
            break;
        case 0:
            cout << "Выход из программы." << endl;
            return 0;
        default:
            cout << "Команда не распознана. Пожалуйста, выберите пункт из меню." << endl;
        }
    }
}