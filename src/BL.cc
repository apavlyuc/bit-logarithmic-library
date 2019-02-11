#include "../inc/BL.hh"

#include <cstdlib>
#include <utility>
#include <iterator>
#include <functional>

using std::to_string;

static bool	optimise_list_bl(list<int>& vec);

/*
**			constructors / destructors
*/

BL::BL(long long num)
{
	_num_str_dec = to_string(num);
	_is_num_str_dec_actual = true;
}

BL::BL(double num)
{
	_num_str_dec = to_string(num);
	_is_num_str_dec_actual = true;
}

BL::BL(unsigned long long num)
{
	_num_str_dec = to_string(num);
	_is_num_str_dec_actual = true;
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
		_num_str_dec = num_str;
		_is_num_str_dec_actual = true;
	}
}

BL::BL(BL const& bl)
{
	if (bl._is_num_str_dec_actual)
	{
		_num_str_dec = bl._num_str_dec;
		_is_num_str_dec_actual = true;
	}

	if (bl._is_num_str_bl_actual)
	{
		_num_str_bl = bl._num_str_bl;
		_is_num_str_bl_actual = true;
	}

	if (bl._is_num_list_bl_actual)
	{
		_num_list_bl = bl._num_list_bl;
		_precision = bl._precision;
		_sign = bl._sign;
		_is_num_list_bl_actual = true;
	}
}

/*
**			public methods
*/

string	BL::get_decimal_form() noexcept
{
	if (!_is_num_str_dec_actual)
		actualize_num_str_dec();

	return _num_str_dec;
}

string	BL::get_bl_form() noexcept
{
	if (!_is_num_str_bl_actual)
		actualize_num_str_bl();

	return _num_str_bl;
}

int		BL::get_precision() const noexcept
{
	return _precision;
}

void	BL::set_precision(int precision) noexcept
{
	_precision = precision;
}

/*
**			private methods
*/

void	BL::actualize_num_str_dec() noexcept // need code
{
	if (_is_num_str_dec_actual)
		return;

	if (!_is_num_list_bl_actual)
		actualize_num_vector_bl();
}

void	BL::actualize_num_str_bl() noexcept // need code
{
	if (_is_num_str_bl_actual)
		return;

	if (_is_num_str_dec_actual)
	{
		// TODO:
	}

	if (_is_num_list_bl_actual)
	{
		// TODO:
	}
}

void	BL::actualize_num_vector_bl() noexcept
{
	if (_is_num_list_bl_actual)
		return;

	if (!_is_num_str_bl_actual)
		actualize_num_str_bl();

	// determine sign
	_sign = _num_str_bl[0] == '0' ? true : false;

	// determine precision
	_precision = std::atoi(_num_str_bl.c_str() + 2);

	// _num_str_bl without sign and precision
	string nums_str = _num_str_bl.substr(1 + _num_str_bl.find('.', 2));

	size_t pos = 0;
	char const *ptr = nullptr;
	while (true)
	{
		ptr = nums_str.c_str() + pos;
		if (!*ptr) break;

		int number = std::atoi(ptr);
		_num_list_bl.push_back(number);

		pos = nums_str.find('.', pos + 1);
		if (pos == string::npos)
			break;
		++pos;
	}
	_num_list_bl.sort(std::greater<int>());

	// replace each two similar values (x) in list by one (x + 1)
	while (optimise_list_bl(_num_list_bl));
}

/*
**			operators
*/

bool	operator<(BL const& num1, BL const& num2)
{
	const_cast<BL&>(num1).actualize_num_vector_bl();
	const_cast<BL&>(num2).actualize_num_vector_bl();

	// if both num1 and num2 are zero
	if (num1._precision == 0 && num2._precision == 0)
		return false;
	// if only num1 is zero
	if (num1._precision == 0 && num2._sign == true)
		return true;
	// if only num2 is zero
	if (num1._sign == false && num2._precision == 0)
		return true;

	// if num1 is negative and num2 is positive
	if (num1._sign == false && num2._sign == true)
		return true;

	// if the same sign
	if (num1._sign == num2._sign)
	{
		bool first_powers_comp = false;

		{
			auto it1 = num1._num_list_bl.begin();
			auto it2 = num2._num_list_bl.begin();

			int precision1 = num1._precision;
			int precision2 = num2._precision;
			while (it1 != num1._num_list_bl.end() && precision1 != 0 &&
					it2 != num2._num_list_bl.end() && precision2 != 0)
			{
				if (*it1 != *it2)
					return num1._sign && (*it1 < *it2);

				--precision1;
				--precision2;
				++it1;
				++it2;
			}

			if (it1 == num1._num_list_bl.end() || precision1 == 0)
			{
				if (it2 != num2._num_list_bl.end() && precision2 != 0)
					first_powers_comp = true;
			}
		}

		return num1._sign && first_powers_comp;
	}

	return false;
}

BL		operator+(BL const& num1, BL const& num2)
{
	BL ret;

	// actualize values in vector
	const_cast<BL&>(num1).actualize_num_vector_bl();
	const_cast<BL&>(num2).actualize_num_vector_bl();

	// determine sign
	if (num1._sign && num2._sign)
		ret._sign = true;
	else if (!num1._sign && !num2._sign)
		ret._sign = false;
	else
	{
		if (num1._sign)
			ret._sign = -num2 < num1;
		else
			ret._sign = -num1 < num2;
	}

	// insert list from both nums
	{
		int len_num1 = num1._precision <= num1._num_list_bl.size() ? num1._precision : num1._num_list_bl.size();
		int len_num2 = num2._precision <= num2._num_list_bl.size() ? num2._precision : num2._num_list_bl.size();

		auto it_num1_end = std::begin(num1._num_list_bl);
		auto it_num2_end = std::begin(num2._num_list_bl);

		std::advance(it_num1_end, len_num1);
		std::advance(it_num2_end, len_num2);

		ret._num_list_bl.insert(std::end(ret._num_list_bl), std::begin(num1._num_list_bl), it_num1_end);
		ret._num_list_bl.insert(std::end(ret._num_list_bl), std::begin(num2._num_list_bl), it_num2_end);
	}
	ret._num_list_bl.sort(std::greater<int>());

	// replace each two similar values (x) in list by one (x + 1)
	while (optimise_list_bl(ret._num_list_bl));

	return ret;
}

BL		operator-(BL const& num1, BL const& num2) // need code
{
	BL ret;

	const_cast<BL&>(num1).actualize_num_vector_bl();
	const_cast<BL&>(num2).actualize_num_vector_bl();

	return ret;
}

BL		operator*(BL const& num1, BL const& num2)
{
	BL ret;

	// actualize values in vector
	const_cast<BL&>(num1).actualize_num_vector_bl();
	const_cast<BL&>(num2).actualize_num_vector_bl();

	// determine sign
	if ((num1._sign && num2._sign) || (!num1._sign && !num2._sign))
		ret._sign = true;
	else
		ret._sign = false;

	// insert vector from both nums
	{
		int len_num1 = num1._precision <= num1._num_list_bl.size() ? num1._precision : num1._num_list_bl.size();
		int len_num2 = num2._precision <= num2._num_list_bl.size() ? num2._precision : num2._num_list_bl.size();

		for (auto it1 = num1._num_list_bl.begin(); len_num1 != 0; ++it1, --len_num1)
		{
			for (auto it2 = num2._num_list_bl.begin(); len_num2 != 0; ++it2, --len_num2)
				ret._num_list_bl.push_back(*it1 + *it2);
		}
	}
	ret._num_list_bl.sort(std::greater<int>());

	// replace each two similar values (x) in list by one (x + 1)
	while (optimise_list_bl(ret._num_list_bl));

	return ret;
}

BL		operator/(BL const& num1, BL const& num2) // need code
{
	BL ret;

	const_cast<BL&>(num1).actualize_num_vector_bl();
	const_cast<BL&>(num2).actualize_num_vector_bl();

	return ret;
}

BL		&BL::operator=(BL const& bl)
{
	if (this == &bl)
		return *this;

	if (bl._is_num_str_dec_actual)
	{
		_num_str_dec = bl._num_str_dec;
		_is_num_str_dec_actual = true;
	}

	if (bl._is_num_str_bl_actual)
	{
		_num_str_bl = bl._num_str_bl;
		_is_num_str_bl_actual = true;
	}

	if (bl._is_num_list_bl_actual)
	{
		_num_list_bl = bl._num_list_bl;
		_sign = bl._sign;
		_precision = bl._precision;
		_is_num_list_bl_actual = true;
	}

	return *this;
}

BL		BL::operator-() const noexcept
{
	BL ret = *this;
	ret.actualize_num_vector_bl();

	ret._sign = false;

	return ret;
}

BL		BL::operator+() const noexcept
{
	return *this;
}

BL::operator string() noexcept
{
	return get_bl_form();
}

/*
**			functions
*/

bool	optimise_list_bl(list<int>& vec)
{
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		auto next = it;
		next++;
		if (*it == *next)
		{
			*it += 1;
			vec.erase(next);
			return true;
		}
	}
	return false;
}