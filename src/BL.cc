#include "../inc/BL.hh"

#include <utility>

using std::to_string;

BL::BL(long long num)
{
	_num_str_bin = to_string(num);
	_is_num_str_bin_actual = true;
}

BL::BL(double num)
{
	_num_str_bin = to_string(num);
	_is_num_str_bin_actual = true;
}

BL::BL(unsigned long long num)
{
	_num_str_bin = to_string(num);
	_is_num_str_bin_actual = true;
}

BL::BL(string const& num_str, bool is_bl_form)
{
	if (is_bl_form)
	{
		_num_str_bl = num_str;
		_is_num_str_bl_actual = true;
	}
	else
	{
		_num_str_bin = num_str;
		_is_num_str_bin_actual = true;
	}
}

BL::BL(BL const& bl)
{
	if (bl._is_num_str_bin_actual)
	{
		_num_str_bin = bl._num_str_bin;
		_is_num_str_bin_actual = true;
	}
	
	if (bl._is_num_str_bl_actual)
	{
		_num_str_bl = bl._num_str_bl;
		_is_num_str_bl_actual = true;
	}

	if (bl._is_num_vector_bl_actual)
	{
		_num_vector_bl = bl._num_vector_bl;
		_accuracy = bl._accuracy;
		_sign = bl._sign;
		_is_num_vector_bl_actual = true;
	}
}

/*
**			public functions
*/

string	BL::get_binary_form() noexcept
{
	if (_is_num_str_bin_actual)
		return std::move(_num_str_bin);
	
	actualize_num_str_bin();

	return std::move(_num_str_bin);
}

string	BL::get_bl_form() noexcept
{
	if (_is_num_str_bl_actual)
		return std::move(_num_str_bl);
	
	actualize_num_str_bl();
	
	return std::move(_num_str_bl);
}

int		BL::get_accuracy() const noexcept
{
	return _accuracy;
}

void	BL::set_accuracy(int accuracy) noexcept
{
	_accuracy = accuracy;
}

/*
**			private functions
*/

void	BL::actualize_num_str_bin() noexcept
{

}

void	BL::actualize_num_str_bl() noexcept
{

}
