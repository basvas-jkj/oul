#include <iostream>

#include "args.hpp"

using namespace std;

namespace oul
{
	void write_short_help()
	{
		cout << "Commands:" << endl;
		for (COMMAND& c : COMMAND::all())
		{
			cout << "    " << c.get_name() << c.get_short_help() << endl;
		}
	}
}
