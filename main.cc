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
	std::cout << "lhs: "; lhs.print_info();
	std::cout << "rhs: "; rhs.print_info();

	BigNumber result = lhs * rhs;
	std::cout << "result: "; result.print_info();
	
	return (0);
}