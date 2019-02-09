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
	BL(string const& num_str, bool is_bl_form = false);
	BL(BL const& bl);
	~BL() = default;

private:
	string		_num_str_bin;
	bool		_is_num_str_bin_actual = false;

	string		_num_str_bl;
	bool		_is_num_str_bl_actual = false;

	vector<int>	_num_vector_bl;
	bool		_sign;
	int			_accuracy;
	bool		_is_num_vector_bl_actual = false;
};
