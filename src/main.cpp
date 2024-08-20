#include <iostream>

#include "config.hpp"
#include "server.hpp"
#include "cmd/args.hpp"
#include "component_manager.hpp"

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
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
				add_files(c, name, source_files, test_files, doc_files);
			}
			else
			{
				string save_as = a.get_option("-as");
				string url = a.get_option("-url");
				string where = a.get_option("-w");
				add_component(c, name, save_as, url, where);
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
				string where = a.get_option("-w");

				create_component(c, name, where, source_files, test_files, doc_files);
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
		else if (a.is("register"))
		{
			registration(c);
		}
	}
	
	return 0;
}
