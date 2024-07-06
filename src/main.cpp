#include <set>
#include <map>
#include <deque>
#include <iostream>
#include <filesystem>

#include "zip.hpp"
#include "help.hpp"
#include "config.hpp"
#include "download.hpp"

using namespace std;
using namespace oul;

class ARGS
{
public:
	enum COMMAND_TYPE { none, init, add, create, list };

private:
	COMMAND_TYPE command;
	set<string> options;
	deque<string> arguments;

public:
	ARGS(int argc, char* argv[]): command(none)
	{
		const map<string, COMMAND_TYPE> command_names =
		{
			{"init", ARGS::init},
			{"add", ARGS::add},
			{"create", ARGS::create},
			{"list", ARGS::list}
		};

		vector<string> args(argv + 1, argv + argc);
		for (string& s : args)
		{
			if (s == "")
			{
				cerr << ":-)" << endl;
			}
			else if (s[0] == '-')
			{
				options.insert(move(s));
			}
			else if (command == none)
			{
				auto i = command_names.find(s);
				if (i == command_names.end())
				{
					cerr << "First non-option argument should be a valid command." << endl;
				}
				else
				{
					command = i->second;
				}
			}
			else
			{
				arguments.push_back(move(s));
			}

		}
	}
	string next_arg()
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
	bool is(COMMAND_TYPE command) const
	{
		return command == this->command;
	}
	bool has_options(const string& option) const
	{
		return options.contains(option);
	}
};

int main(int argc, char* argv[])
{
	ARGS a(argc, argv);
	string conf = CONFIG::find();

	if (a.is(ARGS::none))
	{
		write_short_help();
	}
	else if (a.is(ARGS::init))
	{
		CONFIG::initialize();
	}
	else if (conf == "")
	{
		cerr << "Configuration file not found." << endl;
		cerr << "Call „oul init“ or move into an initialized project." << endl;
		return 1;
	}
	else
	{
		auto [valid, c] = CONFIG::read(conf);
		if (!valid)
		{
			cerr << "Configuration file is corrupted." << endl;
			cerr << "Remove it and initialize again or move into another project.";
			return 1;
		}
		else if (a.is(ARGS::add))
		{
			string arg = a.next_arg();
			if (arg == "")
			{
				cerr << "No component to download." << endl;
				cerr << "Call this command with one additional argument." << endl;
				return 2;
			}

			string url = c.get_url(arg);
			string component = download(url);

			if (component != "")
			{
				ZIP_COMPONENT zip(component);
				vector<string> content(zip.unzip(component, ""));
				arg = a.next_arg();

				c.add_component(arg, url, content);
				filesystem::remove(component);
			}
		}
		else if (a.is(ARGS::list))
		{
			c.list_components();
		}
	}
	
	return 0;
}
