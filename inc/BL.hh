#pragma once

#include <string>
#include <list>

using std::string;
using std::list;

class BL
{
public:
	BL(long long num = 0);
	BL(double num);
	BL(unsigned long long num);
	BL(string const& num_str, bool is_bl_form = false);
	BL(BL const& bl);
	~BL() = default;

	string		get_binary_form() noexcept;
	string		get_bl_form() noexcept;

	int			get_accuracy() const noexcept;
	void		set_accuracy(int accuracy) noexcept;

	BL			&operator=(BL const& bl);
	BL			operator-() const noexcept;
	BL			operator+() const noexcept;

	friend bool	operator<(BL const& num1, BL const& num2);
	friend BL	operator+(BL const& num1, BL const& num2);
	friend BL	operator-(BL const& num1, BL const& num2);
	friend BL	operator*(BL const& num1, BL const& num2);
	friend BL	operator/(BL const& num1, BL const& num2);

	operator string() noexcept;

private:
	void		actualize_num_str_bin() noexcept;
	void		actualize_num_str_bl() noexcept;
	void		actualize_num_vector_bl() noexcept;

	string		_num_str_bin;
	bool		_is_num_str_bin_actual = false;

	string		_num_str_bl;
	bool		_is_num_str_bl_actual = false;

	list<int>	_num_list_bl;
	bool		_sign;
	int			_accuracy;
	bool		_is_num_list_bl_actual = false;
};
