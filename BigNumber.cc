#include "BigNumber.hh"

#include <iostream>
#include <iomanip>

#include <cmath>
#include <algorithm>
#include <list>
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
	std::sort(_vec.begin(), _vec.end());
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
	std::sort(ret._vec.begin(), ret._vec.end());

	while (replace_same(ret._vec));

	ret._precision = ret._vec.size();

	return (ret);
}

BigNumber	operator-(BigNumber const& lhs, BigNumber const& rhs) {
	using Sign = BigNumber::Sign;

	BigNumber difference;

	// Handling corner cases
	// Check if either of operands are zero
	if (0 == lhs._precision || 0 == rhs._precision) {
		if (0 == lhs._precision && 0 == rhs._precision) {
			return difference;
		}

		difference = (0 == rhs._precision) ? lhs : rhs; 
		difference._sign = (0 == rhs._precision)
			? lhs._sign
			: !rhs._sign;
		return difference;
	}

	// Check signs
	if (Sign::NEGATIVE == lhs._sign || Sign::NEGATIVE == rhs._sign) {
		if (Sign::NEGATIVE == rhs._sign) {
			// lhs - -rhs results in lhs + rhs
			auto tmp = -rhs;
			difference = lhs + tmp;
		} else {
			// -lhs - rhs (rhs could not be negative) results in -(lhs + rhs)
			auto tmp = -lhs;
			difference = tmp + rhs;
			difference._sign = Sign::NEGATIVE;	
		}
		return difference;
	}

	BigNumber minuend, subtrahend;
	minuend = std::max(lhs, rhs);
	subtrahend = std::min(lhs, rhs);

	for (int i = 0; i < subtrahend._precision; ++i) {
		std::cout << "before find" << std::endl;
		auto minuend_element =  std::find(minuend._vec.begin(),
			minuend._vec.end(), subtrahend[i]);
		std::cout << "after find" << std::endl;
		// TODO: Talk about moving vector to set (upper/lower bound)
		if (minuend._vec.end() == minuend_element) {
			std::cout << "after find" << std::endl;
			for (int i = minuend._vec.size() - 1; i >= 0; --i) {
				if (minuend[i] > subtrahend[i]) {
					minuend_element = minuend._vec.begin() + i;
					std::list<int> els(minuend[i] - subtrahend[i]);
					std::generate(els.begin(), els.end(),
						[n = minuend[i] - 1]() mutable { return n--; });

				 	std::insert_iterator<std::vector<int>>
						inserter(minuend._vec, minuend_element);

					std::copy(els.begin(), els.end(), inserter);
					break;
				}
			}
		}
		minuend._vec.erase(minuend_element);
	}

	std::cout << "minuend._vec.size(): " << minuend._vec.size() << std::endl; 

	// difference._vec = minuend._vec;
	// difference._precision = minuend._vec.size();
	// difference._sign = (lhs >= rhs);

	return difference;
}

BigNumber	operator*(BigNumber const& obj1, BigNumber const& obj2)
{
	(void)obj1;
	(void)obj2;
	return BigNumber();
}

BigNumber	operator/(BigNumber const& obj1, BigNumber const& obj2)
{
	(void)obj1;
	(void)obj2;
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

// TODO: fix << issues
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

	for (size_t i = 0; i < bits; ++i)
	{
		if (nbr & 1)
		{
			ret.push_back(i);
		}
		nbr >>= 1;
	}

	std::sort(ret.begin(), ret.end());

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