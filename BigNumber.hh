#pragma once

#include <string>
#include <vector>
#include <algorithm>

class BigNumber
{
public:
	BigNumber(int nbr = 0);
	BigNumber(long long nbr);
	BigNumber(size_t nbr);
	BigNumber(std::string const& nbr);
	BigNumber(std::string&& nbr);
	BigNumber(BigNumber const& obj);
	BigNumber(BigNumber&& obj);

	BigNumber	&operator=(BigNumber const& obj);
	BigNumber	&operator=(BigNumber&& obj);
	BigNumber	operator-() const;
	BigNumber	operator+() const;

	const int	&operator[](size_t index) const;

	operator bool() const;

	friend BigNumber	operator+(BigNumber const& obj1, BigNumber const& obj2);
	friend BigNumber	operator-(BigNumber const& obj1, BigNumber const& obj2);// need code
	friend BigNumber	operator*(BigNumber const& obj1, BigNumber const& obj2);
	friend BigNumber	operator/(BigNumber const& lhs, BigNumber const& rhs);// need code

	friend bool	operator<(BigNumber const& lhs, BigNumber const& rhs);
	friend bool	operator>(BigNumber const& obj1, BigNumber const& obj2);
	friend bool	operator>=(BigNumber const& obj1, BigNumber const& obj2);
	friend bool	operator==(BigNumber const& obj1, BigNumber const& obj2);

	friend std::ostream& operator<<(std::ostream& out, BigNumber const& num);
	friend std::istream& operator>>(std::istream& in, BigNumber& num);

	void		print_info();// temp

	inline int				get_precision() const { return _precision; }
	inline bool				get_sign() const { return _sign; }
	inline std::vector<int>	get_vec() const { return _vec; }

private:
	enum Sign {
		POSITIVE = true,
		NEGATIVE = false
	};

	static constexpr int	_LEN = 3;

	std::vector<int>	_vec;
	bool				_sign;
	int					_precision;

	template <typename T, T t>
	void	initialize_from_number() {
		_sign = t >= 0;
		_vec = get_bl_vec(t);
		_precision = _vec.size();
		std::sort(_vec.begin(), _vec.end(), std::greater<int>());
	}
};
