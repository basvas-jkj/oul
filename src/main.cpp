#include <iostream>
#include <memory>

import args;
import common;
import message;

using namespace std;
using namespace oul;

namespace YAML
{
	class ParserException;
}

int main(int argc, char* argv[])
{
	try
	{
		init_messages(argv[0]);
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
	catch (YAML::ParserException& e)
	{
		report_error(ERROR::invalid_configuration_format);
	}
	catch (...)
	{
		report_error(ERROR::unexpected_error);
	}

	return 1;
}
