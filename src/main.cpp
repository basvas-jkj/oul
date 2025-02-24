import std;
import args;
import common;
import component_manager;

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
	try
	{
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
	catch (...)
	{
		println("Unknown internal error.");
	}

	return 1;
}
