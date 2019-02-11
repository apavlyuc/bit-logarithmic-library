#include "../inc/BL.hh"

int main()
{
    BL first_num;
    BL second_num (first_num);
    first_num = second_num;

    first_num = 12.002;
    std::cout << first_num << std::endl;
    first_num += (second_num = "1.2.2.1");
    std::cout << first_num << std::endl
        << second_num << std::endl;
    std::cout << "first num is " << (first_num < second_num ? "bigger" : "smaller")
        << " then the second num" << std::endl;
    std::cout << "first num is " << (first_num == second_num ? "equal to" : "not equal to")
        << " the second num" << std::endl;
    return 0;
}
