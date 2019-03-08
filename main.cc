#include "BigNumber.hh"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::vector;

int			main(int ac, char **av)
{
	if (ac != 3)
		return (0);

	int val1 = std::stoi(av[1]);
	int val2 = std::stoi(av[2]);

	BigNumber num1(val1), num2(val2);

	num1.print_info();
	num2.print_info();
	return (0);
}