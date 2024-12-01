export module args;

import <map>;
import <memory>;
import <string>;
import <tuple>;
import <vector>;

import usings;
import command;
import init;

using namespace std;
using namespace oul;

static tuple<string, string> parse_option(cr<string> arg)
{
	size_t index = arg.find('=');
	
	if (index == string::npos)
	{
		return tuple(arg, "");
	}
	else
	{
		string name = arg.substr(0, index);
		string value = arg.substr(index + 1);
		return tuple(move(name), move(value));
	}
}

export namespace oul
{
	unique_ptr<COMMAND> read_args(int argc, char* argv[])
	{
		vector<string> args(argv + 1, argv + argc);

		int start = 1;
		unique_ptr<COMMAND> command;
		if (args[0] == "init")
		{
			command = make_unique<INIT>();
		}
		else
		{
			return nullptr;
		}

		OPTIONS opt;
		vector<string> arguments;

		for (int f = start; f < args.size(); f += 1)
		{
			string& arg = args[f];

			if (arg[0] == '-')
			{
				auto [name, value] = parse_option(arg);
				opt.add(name, value);
			}
			else
			{
				arguments.push_back(move(arg));
			}
		}

		command->init(move(opt), move(arguments));
		return command;
	}
}