#include "../inc/BL.hh"

#include <utility>
#include <iterator>

using std::to_string;

static bool	optimise_vector_bl(vector<int>& vec);

/*
**			constructors / destructors
*/

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
**			public methods
*/

string	BL::get_binary_form() noexcept
{
	if (!_is_num_str_bin_actual)	
		actualize_num_str_bin();

	return std::move(_num_str_bin);
}

string	BL::get_bl_form() noexcept
{
	if (!_is_num_str_bl_actual)
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
**			private methods
*/

void	BL::actualize_num_str_bin() noexcept // need code
{
	if (_is_num_str_bin_actual)
		return;
}

void	BL::actualize_num_str_bl() noexcept // need code
{
	if (_is_num_str_bl_actual)
		return;
	
	if (_is_num_str_bin_actual)
	{
		// TODO:
	}
	
	if (_is_num_vector_bl_actual)
	{
		// TODO:
	}
}

void	BL::actualize_num_vector_bl() noexcept // need code
{
	if (_is_num_vector_bl_actual)
		return;
	
	if (!_is_num_str_bl_actual)
		actualize_num_str_bl();
}

/*
**			operators
*/

bool	operator<(BL const& num1, BL const& num2)
{
	const_cast<BL&>(num1).actualize_num_vector_bl();
	const_cast<BL&>(num2).actualize_num_vector_bl();

	// if both num1 and num2 are zero
	if (num1._accuracy == 0 && num2._accuracy == 0)
		return false;
	// if only num1 is zero
	if (num1._accuracy == 0 && num2._sign == true)
		return true;
	// if only num2 is zero
	if (num1._sign == false && num2._accuracy == 0)
		return true;

	// if num1 is negative and num2 is positive
	if (num1._sign == false && num2._sign == true)
		return true;
	
	// if the same sign
	if (num1._sign == num2._sign)
	{
		bool first_powers_comp = num1._num_vector_bl[0] < num2._num_vector_bl[0];
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

	// insert vector from both nums
	{
		int len_num1 = num1._accuracy <= num1._num_vector_bl.size() ? num1._accuracy : num1._num_vector_bl.size();
		int len_num2 = num2._accuracy <= num2._num_vector_bl.size() ? num2._accuracy : num2._num_vector_bl.size();

		ret._num_vector_bl.insert(std::end(ret._num_vector_bl), std::begin(num1._num_vector_bl), std::begin(num1._num_vector_bl) + len_num1);
		ret._num_vector_bl.insert(std::end(ret._num_vector_bl), std::begin(num2._num_vector_bl), std::begin(num2._num_vector_bl) + len_num2);
	}

	// replace each two similar values (x) in vector by one (x + 1)
	while (optimise_vector_bl(ret._num_vector_bl));

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
		int len_num1 = num1._accuracy <= num1._num_vector_bl.size() ? num1._accuracy : num1._num_vector_bl.size();
		int len_num2 = num2._accuracy <= num2._num_vector_bl.size() ? num2._accuracy : num2._num_vector_bl.size();
		
		for (int i = 0; i < len_num1; ++i)
		{
			for (int j = 0; i < len_num2; ++j)
				ret._num_vector_bl.push_back(num1._num_vector_bl[i] + num2._num_vector_bl[j]);
		}
	}

	// replace each two similar values (x) in vector by one (x + 1)
	while (optimise_vector_bl(ret._num_vector_bl));

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
		_sign = bl._sign;
		_accuracy = bl._accuracy;
		_is_num_vector_bl_actual = true;
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

bool	optimise_vector_bl(vector<int>& vec)
{
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		if (*it == *(it + 1))
		{
			*it += 1;
			vec.erase(it + 1);
			return true;
		}
	}
	return false;
}