#include <iostream>

#include "server.hpp"
#include "cmd/args.hpp"
#include "config/config.hpp"
#include "component_manager.hpp"

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
	ARGS a(argc, argv);

	if (a.is("none"))
	{
		write_short_help();
	}
	else if (a.is("init"))
	{
		CONFIG::initialize();
	}
	else if (a.is("register") || a.is("list"))
	{
		string config = CONFIG::find();
		if (config == "")
		{
			cerr << "Configuration file not found." << endl;
			cerr << "Call oul init or move to an initialized project." << endl;
			return 1;
		}

		auto [valid, c] = CONFIG::read(config);
		if (!valid)
		{
			cerr << "Configuration file is corrupted." << endl;
			cerr << "Remove it and initialize again or move to another project.";
			return 1;
		}
		else if (a.is("register"))
		{
			registration(c);
		}
		else if (a.is("list"))
		{
			c.list_components();
		}
	}
	else
	{
		auto [success, components] = COMPONENT_MANAGER::init();
		if (!success)
		{
			return 1;
		}
		else if (a.is("add"))
		{
			string name = a.next_arg();
			if (name == "")
			{
				cerr << "No component to download." << endl;
				cerr << "Call this command with one additional argument." << endl;
				return 2;
			}
			else if (a.has_option("-s") || a.has_option("-t") || a.has_option("-d"))
			{
				string source_files = a.get_option("-s");
				string test_files = a.get_option("-t");
				string doc_files = a.get_option("-d");
				components.add_files(name, source_files, test_files, doc_files);
			}
			else
			{
				string save_as = a.get_option("-as");
				string url = a.get_option("-url");
				string where = a.get_option("-w", ".");
				components.add(name, save_as, url, where);
			}
		}
		else if (a.is("remove"))
		{
			string name = a.next_arg();
			if (name == "")
			{
				cerr << "No component to remove." << endl;
				cerr << "Call this command with one additional argument." << endl;
				return 2;
			}
			else if (a.has_option("-s") || a.has_option("-t") || a.has_option("-d"))
			{
				string source_files = a.get_option("-s");
				string test_files = a.get_option("-t");
				string doc_files = a.get_option("-d");
				components.remove_files(name, source_files, test_files, doc_files);
			}
			else
			{
				components.remove(name);
			}
		}
		else if (a.is("create"))
		{
			string name = a.next_arg();

			if (name == "")
			{
				cerr << "Missing name of the component." << endl;
				return 2;
			}
			else
			{
				string source_files = a.get_option("-s");
				string test_files = a.get_option("-t");
				string doc_files = a.get_option("-d");
				string where = a.get_option("-w", ".");

				components.create(name, where, source_files, test_files, doc_files);
			}
		}
		else if (a.is("move"))
		{
			string name = a.next_arg();
			string location = a.next_arg();
			if (name == "")
			{
				cerr << "No component to move." << endl;
				cerr << "Call this command with two additional arguments." << endl;
				return 2;
			}
			else if (location == "")
			{
				cerr << "No location where move to." << endl;
				cerr << "Call this command with one additional argument." << endl;
				return 2;
			}
			else
			{
				components.move(name, location);
			}
		}
		else if (a.is("rename"))
		{
			string old_name = a.next_arg();
			string new_name = a.next_arg();
			components.rename(old_name, new_name);
		}
	}
	
	return 0;
}
