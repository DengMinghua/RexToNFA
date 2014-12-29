#include<iostream>
#include"NFA.h"
int main() {
    while (1) {
        std::string rex = "";
        std::cin >> rex;
        NFA target(rex);
        target.delete_E();
        std::string input;
        target.show_me();
        std::cin >> input;
        while (input != "q!") {
            std::cout << "The match is " << (target.run(input) ? "OK" : "Failed")<<std::endl;
            std::cin >> input;
        }
    }
    system("pause");
}