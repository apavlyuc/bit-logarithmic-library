#include "tests.hh"
#include "BigNumber.hh"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

void 	Flags::read_flags()
{
	for (int i = 1; i < _ac; ++i)
	{
		string tmp = _av[i];

		if (tmp.find("--") != string::npos)
		{
			tests = tmp.find("test") != string::npos;
			// etc.
		}
	}
}

void		tests_for_plus()
{
	{//positive only
		cout << "tests for +: positive " << endl;
		check_res(15, 10, 5, '+');
		check_res(15, 5, 10, '+');
	}
	{//negative only
		cout << "tests for +: negative " << endl;
		check_res(-15, -10, -5, '+');
		check_res(-15, -5, -10, '+');
	}
	{// negative and positive
		cout << "tests for +: negative and positive " << endl;
		check_res(-5, -10, 5, '+');
		check_res(-5, 5, -10, '+');
		check_res(5, 10, -5, '+');
		check_res(5, -5, 10, '+');
	}
	{// with zero
		cout << "tests for +: with zeroes " << endl;
		check_res(0, 0, 0, '+');
		check_res(5, 5, 0, '+');
		check_res(5, 0, 5, '+');
		check_res(-5, -5, 0, '+');
		check_res(-5, 0, -5, '+');
	}
}

void		tests_for_minus()
{
	{//positive only
		cout << "tests for -: positive " << endl;
		check_res(5, 10, 5, '-');
		check_res(-5, 5, 10, '-');
	}
	{//negative only
		cout << "tests for -: negative " << endl;
		check_res(-5, -10, -5, '-');
		check_res(5, -5, -10, '-');
	}
	{// negative and positive
		cout << "tests for -: negative and positive " << endl;
		check_res(-15, -10, 5, '-');
		check_res(15, 5, -10, '-');
		check_res(15, 10, -5, '-');
		check_res(-15, -5, 10, '-');
	}
	{// with zero
		cout << "tests for -: with zeroes " << endl;
		check_res(0, 0, 0, '-');
		check_res(5, 5, 0, '-');
		check_res(-5, 0, 5, '-');
		check_res(-5, -5, 0, '-');
		check_res(5, 0, -5, '-');
	}
}

void		tests_for_mult()
{
	{//positive only
		cout << "tests for *: positive " << endl;
		check_res(50, 10, 5, '*');
		check_res(50, 5, 10, '*');
	}
	{//negative only
		cout << "tests for *: negative " << endl;
		check_res(50, -10, -5, '*');
		check_res(50, -5, -10, '*');
	}
	{// negative and positive
		cout << "tests for *: negative and positive " << endl;
		check_res(-50, -10, 5, '*');
		check_res(-50, 5, -10, '*');
		check_res(-50, 10, -5, '*');
		check_res(-50, -5, 10, '*');
	}
	{// with zero
		cout << "tests for *: with zeroes " << endl;
		check_res(0, 0, 0, '*');
		check_res(0, 5, 0, '*');
		check_res(0, 0, 5, '*');
		check_res(0, -5, 0, '*');
		check_res(0, 0, -5, '*');
	}
}

void		tests_for_div()
{
	{//positive only
		cout << "tests for /: positive " << endl;
		check_res(2, 10, 5, '/');
		check_res(0, 5, 10, '/');
	}
	{//negative only
		cout << "tests for /: negative " << endl;
		check_res(2, -10, -5, '/');
		check_res(0, -5, -10, '/');
	}
	{// negative and positive
		cout << "tests for /: negative and positive " << endl;
		check_res(-2, -10, 5, '/');
		check_res(0, 5, -10, '/');
		check_res(-2, 10, -5, '/');
		check_res(0, -5, 10, '/');
	}
	{// with zero
		cout << "tests for /: with zeroes " << endl;
		check_res(0, 0, 0, '/');
		check_res(0, 5, 0, '/');
		check_res(0, 0, 5, '/');
		check_res(0, -5, 0, '/');
		check_res(0, 0, -5, '/');
	}
}

bool		check_res(int r, int n1, int n2, char op)
{
	BigNumber res(r);
	BigNumber num1(n1);
	BigNumber num2(n2);

	BigNumber tmp;
	switch (op)
	{
		case '+':
		{
			tmp = num1 + num2;
			break;
		}
		case '-':
		{
			tmp = num1 - num2;
			break;
		}
		case '*':
		{
			tmp = num1 * num2;
			break;
		}
		case '/':
		{
			try {
				tmp = num1 / num2;
			}
			catch (string e)
			{
				cout << e << endl;
				return true;
			}
			break;
		}
		default:
			break;
	}

	bool ret = true;

	if (tmp == res)
		cout << "[OK]" << endl;
	else
	{
		cout << n1 << op << n2 << " | [KO]" << endl;
		ret = false;
	}
	return ret;
}
