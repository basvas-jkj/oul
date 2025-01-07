import <iostream>;

import args;
import config;
import component_manager;
import data_file;

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
	unique_ptr<COMMAND> c = read_args(argc, argv);
	if (c != nullptr)
	{
		return c->run();
	}

	return 4;
}
