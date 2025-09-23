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

void createCs(Cs& cs) {
    cout << "\n=== Добавление новой Компрессорной Станции (КС) ===" << endl;
    cout << "Введите название КС: ";
    getline(cin, cs.name);

    cs.totalWorkshops = getValidInput<int>("Введите общее количество цехов: ");
    while (cs.totalWorkshops <= 0) {
        cout << "Количество цехов должно быть положительным. Попробуйте снова.\n";
        cs.totalWorkshops = getValidInput<int>("Введите общее количество цехов: ");
    }

    cs.workingWorkshops = getValidInput<int>("Введите количество цехов в работе: ");
    while (cs.workingWorkshops < 0 || cs.workingWorkshops > cs.totalWorkshops) {
        cout << "Количество рабочих цехов не может быть отрицательным или превышать общее количество. Попробуйте снова.\n";
        cs.workingWorkshops = getValidInput<int>("Введите количество цехов в работе: ");
    }

    cs.efficiency = getValidInput<double>("Введите эффективность КС (%): ");
    while (cs.efficiency < 0 || cs.efficiency > 100) {
        cout << "Эффективность должна быть в диапазоне от 0 до 100%. Попробуйте снова.\n";
        cs.efficiency = getValidInput<double>("Введите эффективность КС (%): ");
    }
    cout << "КС '" << cs.name << "' успешно добавлена!\n" << endl;
}

void displayAllObjects(const Pipe& pipe, const Cs& cs) {
    cout << "\n=== ПРОСМОТР ВСЕХ ОБЪЕКТОВ ===" << endl;

    cout << "--- Труба ---" << endl;
    if (pipe.name.empty()) {
        cout << "Труба не создана." << endl;
    }
    else {
        cout << "Название: " << pipe.name << endl;
        cout << "Длина: " << pipe.length << " км" << endl;
        cout << "Диаметр: " << pipe.diameter << " мм" << endl;
        cout << "В ремонте: " << (pipe.isUnderRepair ? "Да" : "Нет") << endl;
    }

    cout << "\n--- Компрессорная Станция (КС) ---" << endl;
    if (cs.name.empty()) {
        cout << "КС не создана." << endl;
    }
    else {
        cout << "Название: " << cs.name << endl;
        cout << "Всего цехов: " << cs.totalWorkshops << endl;
        cout << "Цехов в работе: " << cs.workingWorkshops << endl;
        cout << "Эффективность: " << cs.efficiency << " %" << endl;
    }
    cout << "--------------------------------" << endl;
}

void editPipe(Pipe& pipe) {
    if (pipe.name.empty()) {
        cout << "Ошибка: Сначала создайте трубу." << endl;
        return;
    }
    cout << "\n=== Редактирование трубы '" << pipe.name << "' ===" << endl;
    cout << "Текущий статус ремонта: " << (pipe.isUnderRepair ? "Да" : "Нет") << endl;
    cout << "1. Включить ремонт" << endl;
    cout << "2. Выключить ремонт" << endl;
    cout << "0. Отмена" << endl;

    int choice = getValidInput<int>("Выберите действие: ");

    switch (choice) {
    case 1:
        pipe.isUnderRepair = true;
        cout << "Ремонт включен." << endl;
        break;
    case 2:
        pipe.isUnderRepair = false;
        cout << "Ремонт выключен." << endl;
        break;
    case 0:
        cout << "Отмена." << endl;
        break;
    default:
        cout << "Неверный выбор." << endl;
    }
}

void editCs(Cs& cs) {
    if (cs.name.empty()) {
        cout << "Ошибка: Сначала создайте КС." << endl;
        return;
    }
    cout << "\n=== Редактирование КС '" << cs.name << "' ===" << endl;
    cout << "Цехов в работе: " << cs.workingWorkshops << " из " << cs.totalWorkshops << endl;
    cout << "1. Запустить цех" << endl;
    cout << "2. Остановить цех" << endl;
    cout << "0. Отмена" << endl;

    int choice = getValidInput<int>("Выберите действие: ");

    switch (choice) {
    case 1:
        if (cs.workingWorkshops < cs.totalWorkshops) {
            cs.workingWorkshops++;
            cout << "Цех запущен. Теперь работает " << cs.workingWorkshops << " цехов." << endl;
        }
        else {
            cout << "Ошибка: Все цехи уже работают." << endl;
        }
        break;
    case 2:
        if (cs.workingWorkshops > 0) {
            cs.workingWorkshops--;
            cout << "Цех остановлен. Теперь работает " << cs.workingWorkshops << " цехов." << endl;
        }
        else {
            cout << "Ошибка: Нет работающих цехов для остановки." << endl;
        }
        break;
    case 0:
        cout << "Отмена." << endl;
        break;
    default:
        cout << "Неверный выбор." << endl;
    }
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
        case 2:
            createCs(myCs);
            break;
        case 3:
            displayAllObjects(myPipe, myCs);
            break;
        case 4:
            editPipe(myPipe);
            break;
        case 5:
            editCs(myCs);
            break;
        case 0:
            cout << "Выход из программы." << endl;
            return 0;
        default:
            cout << "Команда не распознана. Пожалуйста, выберите пункт из меню." << endl;
        }
    }
}