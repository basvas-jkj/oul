#include <memory>

import argument_parser;
import message;
import support;

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
	try
	{
		init_messages();
		unique_ptr<COMMAND> c = read_args(argc, argv);
		if (c != nullptr && c->check())
		{
			c->run();
			return 0;
		}
	}
	catch (CommonException& e)
	{
		e.report();
	}
	catch (ArgumentException& e)
	{
		e.report();
	}
	catch (...)
	{
		report_error(MESSAGE::unexpected_error);
	}

	return 1;
}
