#include "BigNumber.hh"

#include <iostream>
#include <iomanip>

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iterator>

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
	_sign = true;

	_vec = get_bl_vec(nbr);

	_precision = _vec.size();
}

// TODO: fix << issues
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

	return *this;
}

BigNumber	&BigNumber::operator=(BigNumber&& obj)
{
	_sign		= obj._sign;
	_precision	= obj._precision;
	_vec		= std::move(obj._vec);

	return *this;
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

BigNumber	&BigNumber::operator+=(BigNumber const&obj)
{
	*this = *this + obj;
	return *this;
}

BigNumber	&BigNumber::operator-=(BigNumber const&obj)
{
	*this = *this - obj;
	return *this;
}

BigNumber	&BigNumber::operator*=(BigNumber const&obj)
{
	*this = *this * obj;
	return *this;
}

BigNumber	&BigNumber::operator/=(BigNumber const&obj)
{
	*this = *this / obj;
	return *this;
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
	BigNumber ret;

	if (!obj2._sign)
	{
		ret = obj1 + -obj2;
		return std::move(ret);
	}
	if (!obj1._sign)
	{
		ret = -obj1 + obj2;
		ret._sign = false;
		return std::move(ret);
	}
	if (obj1 < obj2)
	{
		ret = obj2 - obj1;
		ret._sign = false;
		return std::move(ret);
	}

	ret._vec = obj1._vec;

	for (auto it = obj2._vec.rbegin(); it != obj2._vec.rend(); ++it)
	{
		auto bigger = std::lower_bound(ret._vec.rbegin(), ret._vec.rend(), *it);
		if (*bigger < *it)
		{
			assert(*bigger < *it);
		}

		int upper_border = *bigger;
		int lowwer_border = *it;
		ret._vec.erase(std::next(bigger).base());

		// cout << "upper: " << upper_border << endl;
		// cout << "lower: " << lowwer_border << endl;

		while (--upper_border >= lowwer_border)
			ret._vec.push_back(upper_border);
		
		std::sort(ret._vec.begin(), ret._vec.end(), std::greater<int>());
	}

	while (replace_same(ret._vec));

	ret._precision = ret._vec.size();
	
	return std::move(ret);
}

BigNumber	operator*(BigNumber const& obj1, BigNumber const& obj2)
{
	BigNumber ret;

	if ((!obj1._sign && obj2._sign) || (obj1._sign && !obj2._sign))
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

BigNumber	operator/(BigNumber const& lhs, BigNumber const& rhs)
{
	if (!rhs._precision) {
		throw string("division by zero");
	}

	auto diff = 0;
	if (lhs._precision && rhs._precision) {
		diff = lhs[0] - rhs[0];
	}
	
	auto zero = BigNumber();
	auto lhs_mod = lhs < zero ? -lhs : lhs;
	auto rhs_mod = rhs < zero ? -rhs : rhs;

	BigNumber result;
	while (lhs_mod > zero && diff >= 0) {
		BigNumber cpy = rhs_mod;
		std::for_each(cpy._vec.begin(), cpy._vec.end(),
			[&diff](int& r) { r += diff; });

		if (cpy > lhs_mod) {
			--diff;
		} else {
			lhs_mod = lhs_mod - cpy;
			result._vec.push_back(diff); result._precision++;
			diff = lhs_mod[0] - rhs_mod[0];
		}
	}

	result._sign = (!lhs._sign && !rhs._sign) || (lhs._sign && rhs._sign);
	return result;
}

bool	operator<(BigNumber const& lhs, BigNumber const& rhs)
{
	if (lhs._sign != rhs._sign) {
		return lhs._sign < rhs._sign;
	}

	auto size = std::min(lhs._precision, rhs._precision);
	auto i = 0;
	for (; i < size && lhs[i] == rhs[i]; ++i);

	if (i == size) {
		return lhs._sign
			? lhs._precision < rhs._precision
			: lhs._precision > rhs._precision;
	}

	return lhs._sign ? lhs[i] < rhs[i] : lhs[i] > rhs[i];
}

bool	operator>(BigNumber const& obj1, BigNumber const& obj2)
{
	if (obj1 < obj2 || obj1 == obj2)
		return false;

	return true;
}

bool	operator>=(BigNumber const& obj1, BigNumber const& obj2)
{
	if (obj1 > obj2 || obj1 == obj2)
		return true;

	return false;
}

bool	operator<=(BigNumber const& obj1, BigNumber const& obj2)
{
	if (obj1 < obj2 || obj1 == obj2)
		return true;

	return false;
}

bool	operator==(BigNumber const& obj1, BigNumber const& obj2)
{
	if (obj1._precision != obj2._precision)
		return false;

	for (int i = 0; i < obj1._precision; ++i)
	{
		if (obj1[i] != obj2[i])
			return false;
	}

	return true;
}

std::ostream& operator<<(std::ostream& out, BigNumber const& obj)
{
	if (!obj._sign)
		out << '-';

	vector<int>	to_show;
	{
		BigNumber tmp = obj > BigNumber() ? obj : -obj;
		BigNumber thousand(1000);

		while (tmp > thousand)
		{
			BigNumber div_res = tmp / thousand;

			int num = get_int_from_bl(tmp - div_res * thousand);
			to_show.push_back(num);

			tmp /= thousand;
		}
		to_show.push_back(get_int_from_bl(tmp));
		std::reverse(to_show.begin(), to_show.end());
	}

	size_t size = to_show.size();

	out << to_show[0];
	size_t i = 1;
	for (; i < size - 1; ++i)
	{
		out << std::setfill('0') << std::setw(BigNumber::_LEN);
		out << std::to_string(to_show[i]);
	}
	if (i < size)
		out << std::setfill('0') << std::setw(BigNumber::_LEN) << to_show[i];

	return (out);
}

std::istream& operator>>(std::istream& in, BigNumber& num)
{
	string input_str;
	in >> input_str;

	num = BigNumber(input_str);

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
