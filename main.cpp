#include<iostream>
#include"NFA.h"
#include<fstream>
#include<time.h>
int main() {
    while (1) {
        std::string rex = "";
        std::string input;
        std::cin >> input;
        std::ifstream infile(input);
        std::string input_text, temp;
        while (!infile.eof()) {
            infile >> temp;
            input_text.append(temp);
        }
        while (rex != "q!") {
            unsigned t = (unsigned)clock();
            std::cin >> rex;
            NFA target(rex);
            target.delete_E();
            target.show_me();
            target.test(input_text);
            t = (unsigned) clock() - t;
            std::cout << t;
        }
    }
    system("pause");
}