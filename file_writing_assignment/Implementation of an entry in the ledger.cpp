#include <iostream>
#include <fstream>
#include <string>

bool isValidDate(const std::string& date) {
    if (date.length() != 10 || date[2] != '.' || date[5] != '.') {
        return false;
    }

    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));

    if (month < 1 || month > 12) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1]) {
        return false;
    }

    return true;
}

bool isValidAmount(const std::string& amount) {
    int dotCount = 0;

    for (char c : amount) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) {
                return false;
            }
        } else if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}

int main() {
    std::string name, surname, date, amount;

    std::cout << "Enter the recipient's name and surname: ";
    std::cin >> name >> surname;

    std::cout << "Enter the payment date (DD.MM.YYYY): ";
    std::cin >> date;

    while (!isValidDate(date)) {
        std::cout << "Invalid date format. Enter the payment date (DD.MM.YYYY): ";
        std::cin >> date;
    }

    std::cout << "Enter the payment amount (in rubles): ";
    std::cin >> amount;

    while (!isValidAmount(amount)) {
        std::cout << "Invalid amount format. Enter the payment amount (in rubles): ";
        std::cin >> amount;
    }

    std::ofstream outfile("/home/akbar/Desktop/file_writing_assignment/file.txt", std::ios_base::app);
    if (!outfile) {
        std::cerr << "Unable to open the file for writing.\n";
        return 1;
    }

    outfile << name << " " << surname << " " << date << " " << amount << " RUB" << std::endl;
    outfile.close();

    std::cout << "Record successfully added.\n";

    return 0;
}
