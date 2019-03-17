#pragma once

struct Flags
{
	Flags(int ac, char **av) : _ac(ac), _av(av) {}

	void	read_flags();

	bool tests;
private:
	int _ac;
	char **_av;
};


void		tests_for_plus();
void		tests_for_minus();
void		tests_for_mult();
void		tests_for_div();
bool		check_res(int res, int num1, int num2, char op);
