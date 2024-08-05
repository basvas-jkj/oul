#include <map>
#include <deque>
#include <iostream>
#include <filesystem>

#include "help.hpp"
#include "config.hpp"
#include "general.hpp"
#include "component_manager.hpp"

using namespace std;
using namespace oul;

class ARGS
{
public:
	enum COMMAND_TYPE {none, init, add, create, list, rename};

private:
	COMMAND_TYPE command;
	map<string, string> options;
	deque<string> arguments;

public:
	ARGS(int argc, char* argv[]): command(none)
	{
		const map<string, COMMAND_TYPE> command_names =
		{
			{"init", ARGS::init},
			{"add", ARGS::add},
			{"create", ARGS::create},
			{"list", ARGS::list},
			{"rename", ARGS::rename}
		};

		vector<string> args(argv + 1, argv + argc);
		for (std::string& s : args)
		{
			if (s == "")
			{
				cerr << ":-)" << endl;
			}
			else if (s[0] == '-')
			{
				vector<string> split_option = split(s, '=');

				if (split_option.size() == 1)
				{
					options.insert({move(s), ""});
				}
				else
				{
					options.insert({move(split_option[0]), move(split_option[1])});
				}
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
	string get_option(const string& option)
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
			string name = a.next_arg();
			string save_as = a.next_arg();
			if (name == "")
			{
				cerr << "No component to download." << endl;
				cerr << "Call this command with one additional argument." << endl;
				return 2;
			}
			else
			{
				add_component(c, name, save_as);
			}
		}
		else if (a.is(ARGS::create))
		{
			string name = a.next_arg();
			string source_files = a.get_option("-s");
			string test_files = a.get_option("-t");
			string doc_files = a.get_option("-d");

			if (name == "")
			{
				cerr << "Missing name of the component." << endl;
				return 2;
			}
			else
			{
				create_component(c, name, source_files, test_files, doc_files);
			}
		}
		else if (a.is(ARGS::rename))
		{
			string old_name = a.next_arg();
			string new_name = a.next_arg();
			rename_component(c, old_name, new_name);
		}
		else if (a.is(ARGS::list))
		{
			c.list_components();
		}
	}
	
	return 0;
}
