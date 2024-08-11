#include <iostream>

#include "args.hpp"
#include "../general.hpp"

using namespace std;

namespace oul
{
	ARGS::ARGS(int argc, char* argv[])
	{
		COMMAND::init();
		c = &COMMAND::none();

		vector<string> args(argv + 1, argv + argc);
		for (std::string& arg : args)
		{
			if (arg == "")
			{
				cerr << ":-)" << endl;
			}
			else if (arg[0] == '-')
			{
				vector<string> split_option = split(arg, '=');

				if (split_option.size() == 1)
				{
					options.insert({move(arg), ""});
				}
				else
				{
					options.insert({move(split_option[0]), move(split_option[1])});
				}
			}
			else if (*c == COMMAND::none())
			{
				c = &COMMAND::find(arg);
				if (*c == COMMAND::none())
				{
					cerr << "First non-option argument should be a valid command." << endl;
				}
			}
			else
			{
				arguments.push_back(move(arg));
			}

		}
	}

	string ARGS::next_arg()
	{
		if (arguments.size() == 0)
		{
			return "";
		}
		else
		{
			string a = move(arguments.front());
			arguments.pop_front();
			return a;
		}
	}
	bool ARGS::is(const string& name) const
	{
		return c->is(name);
	}
	bool ARGS::has_options(const string& option) const
	{
		return options.contains(option);
	}
	string ARGS::get_option(const string& option) const
	{
		auto i = options.find(option);
		if (i == options.end())
		{
			return "";
		}
		else
		{
			return i->second;
		}
	}
}