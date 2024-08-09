#include <map>
#include <deque>
#include <iostream>
#include <filesystem>

#include "help.hpp"
#include "config.hpp"
#include "command.hpp"
#include "general.hpp"
#include "component_manager.hpp"

using namespace std;
using namespace oul;

class ARGS
{
	COMMAND *c;
	map<string, string> options;
	deque<string> arguments;

public:
	ARGS(int argc, char* argv[]): c(&COMMAND::none())
	{
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
	bool is(const string& name) const
	{
		return c->is(name);
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
	COMMAND::init();
	ARGS a(argc, argv);
	string conf = CONFIG::find();

	if (a.is("none"))
	{
		write_short_help();
	}
	else if (a.is("init"))
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
		else if (a.is("add"))
		{
			string name = a.next_arg();
			string save_as = a.next_arg();
			if (name == "")
			{
				cerr << "No component to download." << endl;
				cerr << "Call this command with one additional argument." << endl;
				return 2;
			}
			else if (a.has_options("-s") || a.has_options("-t") || a.has_options("-d"))
			{
				string source_files = a.get_option("-s");
				string test_files = a.get_option("-t");
				string doc_files = a.get_option("-d");
				add_files(c, name, source_files, test_files, doc_files);
			}
			else
			{
				add_component(c, name, save_as);
			}
		}
		else if (a.is("create"))
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
		else if (a.is("rename"))
		{
			string old_name = a.next_arg();
			string new_name = a.next_arg();
			rename_component(c, old_name, new_name);
		}
		else if (a.is("list"))
		{
			c.list_components();
		}
	}
	
	return 0;
}
