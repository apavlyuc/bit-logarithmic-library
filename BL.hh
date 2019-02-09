#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class BL
{
public:
	BL(long long num = 0);
	BL(double num);
	BL(unsigned long long num);
	BL(string const& num_str, bool bl_form = false);
	BL(BL const& bl);

private:
	string		_num_str_bin;
	bool		_is_num_str_bin_actual;

	string		_num_str_bl;
	bool		_is_num_str_bl_actual;

	vector<int>	_num_vector;
	bool		_is_num_vector_actual;
};
