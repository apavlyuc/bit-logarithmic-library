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

	BigNumber nbr;

	cin >> nbr;
	cout << nbr << endl;

	return 0;
}
