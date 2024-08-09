#include <iostream>

#include "help.hpp"
#include "command.hpp"

using namespace std;

void write_short_help()
{
	cout << "Commands:" << endl;
	for (COMMAND &c : COMMAND::all())
	{
		cout << "    " << c.get_name() << c.get_short_help() << endl;
	}
}