#include <iostream>
#include <optional>
#include <filesystem>

#include "zip.hpp"
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
	vector<string> args(argv + 1, argv + argc);
	if (argc > 1)
	{
		if (args[0] == "init")
		{
			CONFIG::initialize();
		}
		else if (CONFIG::find() == "")
		{
			cerr << "Configuration file not found." << endl;
			return 1;
		}
		else
		{
			optional<CONFIG> c = CONFIG::read();
			if (!c.has_value())
			{
				cerr << "Configuration file is corrupted." << endl;
				return 1;
			}
			else if (args[0] == "add")
			{
				if (argc < 3)
				{
					cerr << "No component to download." << endl;
					cerr << "Call this command with one additional argument." << endl;
					return 2;
				}

				string url = c->get_url(args[1]);
				string component = download(url);

				if (component != "")
				{
                    ZIP_COMPONENT zip(component);


                    vector<string> content(zip.unzip(component, ""));
                    c->add_component(args[2], url, content);
					filesystem::remove(component);
				}
			}
            else if (args[0] == "list")
            {
                c->list_components();
            }
        }
    }
	else
	{
		cout << "Commands:" << endl;
        cout << "    init       creates configuration file" << endl;
        cout << "    add        downloads component and adds it into configuration" << endl;
        cout << "    list       writes list of all installed or localy created components" << endl;
	}

	return 0;
}
