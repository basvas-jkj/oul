#include <iostream>
#include <optional>
#include <filesystem>

#include "zip.hpp"
#include "config.hpp"
#include "download.hpp"

using namespace std;
using namespace oul;

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
					vector<string> content = unzip(component, "");
					c->add_component(url, component, content);
					filesystem::remove(component);
				}
			}
		}
	}
	else
	{
		cout << "Commands:" << endl;
		cout << "    init    creates configuration file" << endl;
		cout << "    add     downloads component and adds it into configuration" << endl;
	}

	return 0;
}
