#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <map>

const int max_notes = 1000;

// Fill the ATM with random banknotes
void fill_atm(std::map<int, int> &notes) {
    int remaining_space = max_notes;
    for (const auto &note : notes) {
        remaining_space -= note.second;
    }

    while (remaining_space > 0) {
        int note = (1 + std::rand() % 49) * 100;
        int num_notes_to_add = std::min(remaining_space, 1 + std::rand() % remaining_space);
        notes[note] += num_notes_to_add;
        remaining_space -= num_notes_to_add;
    }
}

// Attempt to withdraw the specified amount from the ATM
bool withdraw_money(std::map<int, int> &notes, int amount) {
    std::map<int, int> updated_notes = notes;
    for (auto it = updated_notes.rbegin(); it != updated_notes.rend(); ++it) {
        while (amount >= it->first && it->second > 0) {
            amount -= it->first;
            --it->second;
        }
        if (amount == 0) {
            notes = updated_notes;
            return true;
        }
    }
    return false;
}

// Save the ATM state to a binary file
bool save_atm_state(const std::map<int, int> &notes) {
    std::ofstream ofs("atm.bin", std::ios::binary | std::ios::out);
    if (!ofs) {
        std::cerr << "Error: Failed to open atm.bin for writing" << std::endl;
        return false;
    }

    // Write the number of notes to the binary file
    int num_notes = notes.size();
    ofs.write(reinterpret_cast<const char *>(&num_notes), sizeof(int));

    // Write each note to the binary file
    for (const auto &note : notes) {
        ofs.write(reinterpret_cast<const char *>(&note.first), sizeof(int));
        ofs.write(reinterpret_cast<const char *>(&note.second), sizeof(int));
    }

    ofs.close();
    return true;
}

// Load the ATM state from a binary file
bool load_atm_state(std::map<int, int> &notes) {
    std::ifstream ifs("atm.bin", std::ios::binary | std::ios::in);
    if (!ifs) {
        std::cerr << "Error: Failed to open atm.bin for reading" << std::endl;
        return false;
    }

    // Read the number of notes from the binary file
    int num_notes;
    ifs.read(reinterpret_cast<char *>(&num_notes), sizeof(int));

    // Read each note from the binary file
    for (int i = 0; i < num_notes; ++i) {
        int note, count;
        ifs.read(reinterpret_cast<char *>(&note), sizeof(int));
        ifs.read(reinterpret_cast<char *>(&count), sizeof(int));
        notes[note] = count;
    }

    ifs.close();
    return true;
}

// Display the ATM contents
void display_atm_contents(const std::map<int, int> &notes) {
    std::cout << "ATM contents:" << std::endl;
    for (const auto &note : notes) {
        std::cout << "Banknote: " << note.first << " rubles, Quantity: " << note.second << std::endl;
    }
}

int main() {
    std::srand(std::time(nullptr));
        std::map<int, int> notes;
    if (!load_atm_state(notes)) {
        // ATM state could not be loaded, fill with random banknotes
        fill_atm(notes);
    }

    char choice;
    std::cout << "Enter + to fill the ATM or - to withdraw money: ";
    std::cin >> choice;

    if (choice == '+') {
        // Fill the ATM
        fill_atm(notes);
        display_atm_contents(notes);
        if (!save_atm_state(notes)) {
            std::cerr << "Failed to save ATM state" << std::endl;
        }
    } else if (choice == '-') {
        // Withdraw money from the ATM
        int amount_to_withdraw;
        std::cout << "Enter amount to withdraw (in multiples of 100 rubles): ";
        std::cin >> amount_to_withdraw;
        if (amount_to_withdraw % 100 != 0) {
            std::cerr << "Error: Amount must be a multiple of 100 rubles" << std::endl;
            return 1;
        }

        if (withdraw_money(notes, amount_to_withdraw)) {
            std::cout << "Withdrawal successful" << std::endl;
            display_atm_contents(notes);
            if (!save_atm_state(notes)) {
                std::cerr << "Failed to save ATM state" << std::endl;
            }
        } else {
            std::cerr << "Error: Unable to withdraw the specified amount" << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Error: Invalid choice" << std::endl;
        return 1;
    }

    return 0;
}

   
