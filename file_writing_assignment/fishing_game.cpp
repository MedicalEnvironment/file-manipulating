#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string fish_to_catch;
    std::cout << "Enter the type of fish you want to catch: ";
    std::getline(std::cin, fish_to_catch);

    std::fstream river("river.txt");
    if(!river) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::ofstream basket("basket.txt", std::ios::app);
    if(!basket) {
        std::cerr << "Unable to open basket.txt file" << std::endl;
        return 1;
}

std::string fish;
int fish_caught = 0;
while(std::getline(river, fish)) {
    if(fish == fish_to_catch) {
        basket << fish << std::endl;
        ++fish_caught;
    }
}


river.close();
basket.close();

std::cout << "You caught " << fish_caught << " " << fish_to_catch << "(s)." << std::endl;

return 0;

}