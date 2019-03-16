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
	if (3 != args_count) {
		std::cout << "usage: " << args[0] << " arg1 arg2" << std::endl;
		return 1;
	}

	BigNumber lhs, rhs;

	lhs = std::stoi(args[1]); rhs = std::stoi(args[2]);
	std::cout << "lhs:\n"; lhs.print_info(); std::cout << "\n";
	std::cout << "rhs:\n"; rhs.print_info(); std::cout << "\n";

	// std::cout << "lhs - rhs: " << (lhs < rhs ? "yes" : "no") << "\n";
	std::cout << "lhs / rhs:\n"; (lhs / rhs).print_info();	
	return 0;
}