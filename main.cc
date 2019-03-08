#include "BigNumber.hh"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::vector;

int			main(int args_count, char **args)
{
<<<<<<< HEAD
	if (ac != 3)
		return (0);

	int val1 = std::stoi(av[1]);
	int val2 = std::stoi(av[2]);

	BigNumber num1(val1), num2(val2);

	num1.print_info();
	num2.print_info();
=======
	if (3 != args_count) {
		std::cout << "usage: " << args[0] << " arg1 arg2" << std::endl;
		return 1;
	}

	BigNumber lhs, rhs;

	lhs = std::stoi(args[1]); rhs = std::stoi(args[2]);
	std::cout << "lhs: "; lhs.print_info();
	std::cout << "rhs: "; rhs.print_info();

	BigNumber result = lhs - rhs;
	std::cout << "result: "; result.print_info();
	
>>>>>>> f34eb22493603eaf8af4a74223a2a134a59be82f
	return (0);
}