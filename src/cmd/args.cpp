#include <iostream>

#include "args.hpp"
#include "../general.hpp"

using namespace std;

namespace oul
{
	string convert_to_short(const string& opt)
	{
		map<string, string> short_variants =
		{
			{"-new", "-n"},
			{"-where", "-w"},
			{"-src", "-s"},
			{"-test", "-t"},
			{"-doc", "-d"},
			{"-version", "-v"}
		};

		auto i = short_variants.find(opt);
		if (i == short_variants.end())
		{
			return opt;
		}
		else
		{
			return i->second;
		}
	}

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
					string short_option = convert_to_short(arg);
					options.insert({move(short_option), ""});
				}
				else
				{
					string short_option = convert_to_short(split_option[0]);
					options.insert({move(short_option), move(split_option[1])});
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
	bool ARGS::has_option(const string& option) const
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