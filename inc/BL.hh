#pragma once

#include <iostream>
#include <string>
#include <list>

using std::string;
using std::list;
using std::ostream;
using std::istream;

class BL
{
public:
	BL(long long num = 0);
	BL(double num);
	BL(unsigned long long num);
	BL(string const& num_str, bool is_bl_form = false);
	BL(BL const& bl);
	~BL() = default;

	string			get_decimal_form() noexcept;
	string			get_bl_form() noexcept;

	int				get_precision() const noexcept;
	void			set_precision(int precision) noexcept;

	BL				&operator=(BL const& bl);
	BL				operator-() const noexcept;
	BL				operator+() const noexcept;

	friend bool		operator<(BL const& num1, BL const& num2);
	friend BL		operator+(BL const& num1, BL const& num2);
	friend BL		operator-(BL const& num1, BL const& num2);
	friend BL		operator*(BL const& num1, BL const& num2);
	friend BL		operator/(BL const& num1, BL const& num2);
	friend ostream	&operator<<(ostream& out, BL const& obj);
	friend istream	&operator>>(istream& in, BL& obj);

	operator string() noexcept;

private:
	void		actualize_num_str_dec() noexcept;
	void		actualize_num_str_bl() noexcept;
	void		actualize_num_list_bl() noexcept;

	string		_num_str_dec;
	bool		_is_num_str_dec_actual = false;

	string		_num_str_bl;
	bool		_is_num_str_bl_actual = false;

	list<int>	_num_list_bl;
	bool		_sign;
	int			_precision;
	bool		_is_num_list_bl_actual = false;
};
