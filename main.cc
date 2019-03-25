#include "BigNumber.hh"
#include "tests.hh"

#include <iostream>
#include <string>
#include <utility>

#include <future>

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::vector;
using std::pair;

void make_some_shit(BigNumber num, BigNumber sqr, BigNumber min, BigNumber max);

int			main(int ac, char **av)
{
	// Flags obj(ac, av);
	// obj.read_flags();

	// if (obj.tests)
	// {
	// 	tests_for_plus();
	// 	tests_for_minus();
	// 	tests_for_mult();
	// 	tests_for_div();
	// }

	// BigNumber num1, num2;
	// cin >> num1;
	// cin >> num2;
	// cout << num1 * num2 << endl;

	BigNumber num, min, max, sqr;

	// num = BigNumber("114381625757888867669235779976146612010218296721242362562561842935706935245733897830597123563958705058989075147599290026879543541");
					//	5210644015679228794060694325390955853335898483908056458352183851018372555735221
					// 	886545139418875782276278402253231310598662201538434863646654518619355872298587837207211628443950670908391150581710667
					//	
	// min = BigNumber("972266780371497408477605202563153434932205907159926570804385605");
	// max = BigNumber("1944533560742994816955210405126306869864411814319853141608771210");
	// sqr = BigNumber("10694934584086471525314207693308900296322993593605128511616736585");

	cin >> num;
	sqr = sqrt(num);
	if (sqr == BigNumber("10694934584086471525314207693308900296322993593605128511616736585"))
	{
		min = BigNumber("972266780371497408477605202563153434932205907159926570804385605");
		max = BigNumber("1944533560742994816955210405126306869864411814319853141608771210");
	}
	else
	{
		min = BigNumber("1");
		max = BigNumber(sqr - BigNumber("1"));
	}

	make_some_shit(num, sqr, min, max);

	// BigNumber tmp = min + (max - min) / BigNumber(2);
	// std::future<void> ans1 = std::async(&make_some_shit, num, sqr, min, tmp);
	// std::future<void> ans2 = std::async(&make_some_shit, num, sqr, tmp, max);

	// ans1.get();
	// ans2.get();

	return 0;
}
//972266780371497408477605202563153434932205907159926570804385605...1944533560742994816955210405126306869864411814319853141608771210
//114381625757888867669235779976146612010218296721242362562561842935706935245733897830597123563958705058989075147599290026879543541

void make_some_shit(BigNumber num, BigNumber sqr, BigNumber min, BigNumber max)
{
	pair<BigNumber, BigNumber> ret;

	BigNumber denominator = sqr - min;
	BigNumber min_denominator = sqr - max - BigNumber(1);
	
	while (denominator > min_denominator)
	{
		BigNumber div_res = num / denominator;
		BigNumber mlt_res = denominator * div_res;

		if (mlt_res == num)
		{
			ret.first = denominator;
			ret.second = div_res;
			break;
		}

		--denominator;
	}

	if (ret.first != BigNumber(0))
	{
		cout << "p: " << ret.first << endl;
		cout << "q: " << ret.second << endl;
	}
}