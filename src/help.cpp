#include <iostream>

#include "help.hpp"

using namespace std;

void write_short_help()
{
	cout << "Commands:" << endl;
	cout << "    init       creates configuration file" << endl;
	cout << "    add        downloads component and adds it into configuration" << endl;
	cout << "    create     creates new component locally" << endl;
	cout << "    list       writes list of all installed or localy created components" << endl;
}