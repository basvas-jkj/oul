module;

#include <boost/filesystem.hpp>

export module common;

export import usings;
import std;
import message;

using namespace std;
using namespace boost::filesystem;

namespace oul
{
    /**
     * @brief Pøesune soubor. Pokud cílová cesta neexistuje, bude vytvoøena.
     * @param source_directory pùvodní umístìní
     * @param target_directory nové umístìní
     * @param entry_name jméno souboru èi složky
     **/
    export void move_file(cr<path> source_directory, cr<path> target_directory, cr<path> entry_name)
	{
		path source = source_directory / entry_name;
		path target = target_directory / entry_name;
		if (target.has_parent_path())
		{
			create_directories(target.parent_path());
		}

		rename(source, target);
	}
	/**
	 * @brief Rozdìlí øetìzec na podøetìzce podle zadaného oddìlovaèe.
	 * @param s rozdìlovaný øetìzec
	 * @param ch oddìlovaè
	 * @return seznam podøetìzcù
	 **/
	vector<string> split(cr<string> s, char ch)
	{
		vector<string> v;
		string::const_iterator start = s.begin();

		while (start != s.end())
		{
			string::const_iterator end = find(start, s.end(), ch);
			v.push_back(string(start, end));

			if (end == s.end())
			{
				break;
			}
			else
			{
				start = end + 1;
			}
		}

		return v;
	}

	export class CommonException: exception
	{
		ERROR name;

	public:
		CommonException(ERROR n): name(n)
		{}
		void report() const
		{
			report_error(name);
		}
	};
}