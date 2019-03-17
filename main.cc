#include "BigNumber.hh"
#include "tests.hh"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::vector;

int			main(int args_count, char **args)
{
	Flags obj(args_count, args);
	obj.read_flags();

	if (obj.tests)
	{
		tests_for_plus();
		tests_for_minus();
		tests_for_mult();
		tests_for_div();
	}

	if (3 != args_count) {
		std::cout << "usage: " << args[0] << " arg1 arg2" << std::endl;
		return 1;
	}

	BigNumber lhs, rhs;
	lhs = args[1]; rhs = args[2];

	lhs.print_info();

	// std::cout << "lhs:\n"; lhs.print_info(); std::cout << "\n";
	// std::cout << "rhs:\n"; rhs.print_info(); std::cout << "\n";

	// std::cout << "lhs - rhs: " << (lhs < rhs ? "yes" : "no") << "\n";
	// std::cout << "lhs / rhs:\n"; (lhs / rhs).print_info();
	return 0;
}
