#include "BigNumber.hh"

#include <iostream>
#include <iomanip>

#include <math.h>
#include <algorithm>

using std::cout;
using std::endl;

using std::string;
using std::vector;

template<typename T>
static vector<int>		get_bl_vec(T nbr);
static int				get_int_from_bl(BigNumber obj);
static bool				replace_same(vector<int>& vec);

/*
**			constructors
*/

BigNumber::BigNumber(int nbr)
{
	_sign = nbr >= 0;

	_vec = get_bl_vec(nbr);

	_precision = _vec.size();
}

BigNumber::BigNumber(long long nbr)
{
	_sign = nbr >= 0;

	_vec = get_bl_vec(nbr);

	_precision = _vec.size();
}

BigNumber::BigNumber(size_t nbr)
{
	_sign = nbr >= 0;

	_vec = get_bl_vec(nbr);

	_precision = _vec.size();
}

BigNumber::BigNumber(string const& nbr)
{
	if (nbr.empty())
	{
		_sign = true;
		_precision = 0;
		return;
	}

	string temp_nbr = nbr;

	// _sign init
	_sign = temp_nbr[0] == '-' ? false : true;
	if (temp_nbr[0] == '-' || temp_nbr[0] == '+')
	{
		temp_nbr[0] = '0';
	}

	{// _vec init
		// big_int init
		vector<int> big_int;
		{
			int len = (int)nbr.length();
			for (int i = len; i > 0; i -= _LEN)
			{
				temp_nbr[i] = 0;
				big_int.push_back(atoi(i >= _LEN ? temp_nbr.c_str() + i - _LEN : temp_nbr.c_str()));
			}
			if (big_int.back() < 0)
				big_int.back() *= -1;
		}

		BigNumber tmp(big_int[0]);
		BigNumber thousand(1000);
		for (size_t i = 1; i < big_int.size(); ++i)
		{
			if (big_int[i] == 0)
			{
				continue;
			}

			BigNumber to_mult(1);
			for (size_t j = 0; j < i; ++j)
			{
				to_mult = to_mult * thousand;
			}

			tmp = tmp + BigNumber(big_int[i]) * to_mult;
		}

		_vec = std::move(tmp._vec);
	}

	_precision = _vec.size();
}

BigNumber::BigNumber(string&& nbr) : BigNumber(nbr)
{}

BigNumber::BigNumber(BigNumber const& obj)
{
	_sign		= obj._sign;
	_precision	= obj._precision;
	_vec		= obj._vec;
}

BigNumber::BigNumber(BigNumber&& obj)
{
	_sign		= obj._sign;
	_precision	= obj._precision;
	_vec		= std::move(obj._vec);
}

/*
**			operators
*/

BigNumber	&BigNumber::operator=(BigNumber const& obj)
{
	_sign		= obj._sign;
	_precision	= obj._precision;
	_vec		= obj._vec;
}

BigNumber	&BigNumber::operator=(BigNumber&& obj)
{
	_sign		= obj._sign;
	_precision	= obj._precision;
	_vec		= std::move(obj._vec);
}

BigNumber	BigNumber::operator+() const
{
	return *this;
}

BigNumber	BigNumber::operator-() const
{
	BigNumber ret(*this);

	ret._sign = !ret._sign;

	return std::move(ret);
}

const int	&BigNumber::operator[](size_t index) const
{
	return _vec[index];
}

BigNumber::operator bool() const
{
	return _precision;
}

BigNumber	operator+(BigNumber const& obj1, BigNumber const& obj2)
{
	BigNumber ret;

	if (!obj1._sign && !obj2._sign)
		ret._sign = false;
	else if (!obj1._sign)
		return obj2 - -obj1;
	else if (!obj2._sign)
		return obj1 - -obj2;

	ret._vec.insert(ret._vec.end(), obj1._vec.begin(), obj1._vec.end());
	ret._vec.insert(ret._vec.end(), obj2._vec.begin(), obj2._vec.end());
	std::sort(ret._vec.begin(), ret._vec.end(), std::greater<int>());

	while (replace_same(ret._vec));

	ret._precision = ret._vec.size();

	return std::move(ret);
}

BigNumber	operator-(BigNumber const& obj1, BigNumber const& obj2)
{
	return BigNumber();
}

BigNumber	operator*(BigNumber const& obj1, BigNumber const& obj2)
{
	BigNumber ret;

	if (!obj1._sign && obj2._sign || obj1._sign && !obj2._sign)
		ret._sign = false;

	for (auto n1 : obj1._vec)
	{
		for (auto n2 : obj2._vec)
		{
			ret._vec.push_back(n1 + n2);
		}
	}
	std::sort(ret._vec.begin(), ret._vec.end(), std::greater<int>());

	while (replace_same(ret._vec));

	ret._precision = ret._vec.size();

	return std::move(ret);
}

BigNumber	operator/(BigNumber const& obj1, BigNumber const& obj2)
{
	return BigNumber();
}

bool	operator<(BigNumber const& obj1, BigNumber const& obj2)
{
	if (!obj1._sign && obj2._sign)
		return true;
	
	if (obj1._sign && obj2._sign)
	{
		for (int i = 0; i < obj1._precision && i < obj2._precision; ++i)
		{
			if (obj1[i] < obj2[i])
				return true;
		}
	}

	if (!obj1._sign && !obj2._sign)
	{
		for (int i = 0; i < obj1._precision && i < obj2._precision; ++i)
		{
			if (obj1[i] > obj2[i])
				return true;
		}
	}

	return false;
}

std::ostream& operator<<(std::ostream& out, BigNumber const& obj)
{
	if (!obj._sign)
		out << '-';

	vector<int>	to_show;
	{// to_show init
		BigNumber tmp = obj;
		BigNumber thousand(1000);

		if (!tmp)
			to_show.push_back(0);
		while (tmp)
		{
			BigNumber div_res = tmp / thousand;

			int num = get_int_from_bl(tmp - div_res);
			to_show.push_back(num);

			tmp = div_res;
		}
		std::reverse(to_show.begin(), to_show.end());
	}

	size_t size = to_show.size();

	out << obj._vec[0];
	for (size_t i = 1; i < size; ++i)
	{
		out << std::setfill('0') << std::setw(BigNumber::_LEN);
		out << to_show[i];
	}

	return (out);
}

std::istream& operator>>(std::istream& in, BigNumber& num)
{
	string input_str;
	in >> input_str;

	num = BigNumber(static_cast<string&&>(input_str));

	return (in);
}

/*
**			public methods
*/

void	BigNumber::print_info()
{
	cout << "_sign: " << (_sign ? '+' : '-') << endl;
	cout << "_precision: " << _precision << endl;
	cout << "_vec: ";

	size_t size = _vec.size();

	for (size_t i = 0; i < size; ++i)
	{
		cout << _vec[i] << '.';
	}
	cout << endl;
}

/*
**			functions
*/
template<typename T>
static vector<int>		get_bl_vec(T nbr)
{
	vector<int>	ret;

	size_t	bits = sizeof(nbr) * 8;

	if (nbr < 0)
		nbr = ~nbr + 1;

	for (size_t i = 0; i < bits; ++i)
	{
		if (nbr & 1)
		{
			ret.push_back(i);
		}
		nbr >>= 1;
	}

	std::sort(ret.begin(), ret.end(), std::greater<int>());

	return ret;
}

static int				get_int_from_bl(BigNumber obj)
{
	int			ret = 0;

	vector<int>	tmp = obj.get_vec();
	size_t		len = obj.get_precision();

	for (size_t i = 0; i < len; ++i)
	{
		ret += static_cast<int>(pow(2, tmp[i]));
	}

	return (ret);
}

static bool				replace_same(vector<int>& vec)
{
	size_t size = vec.size();

	if (size <= 1)
		return false;

	for (size_t i = 0; i < size - 1; ++i)
	{
		if (vec[i] == vec[i + 1])
		{
			vec[i + 1] += 1;
			vec.erase(vec.begin() + i);
			return true;
		}
	}
	return false;
}
