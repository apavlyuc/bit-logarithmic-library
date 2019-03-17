#pragma once

struct Flags
{
	bool tests;

	void	read_flags(int ac, char **av);
};


void		tests_for_plus();
void		tests_for_minus();
void		tests_for_mult();
void		tests_for_div();
bool		check_res(int res, int num1, int num2, char op);
