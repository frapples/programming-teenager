#include<iostream>
#include<string>

int main(void)
{
    std::string n;

    // while (std::cin >> n) {
    while (!(std::cin >> n).fail()) {
        std::cout << n << std::endl;
    }
    return 0;
}
