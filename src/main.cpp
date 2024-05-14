#include <iostream>
#include <optional>

#include "config.hpp"

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
	vector<string> args(argv + 1, argv + argc);
	if (argc > 1)
	{
		if (args[0] == "init")
		{
			initialize();
		}
	}
	else
	{
		string config_file = find_configaration();

		if (config_file == "")
		{
			cerr << "Configuration file not found." << endl;
			return 0;
		}

		optional<CONFIG> cfg = read_configuration(config_file);
		if (cfg.has_value())
		{
			cout << cfg->name;
		}
		else
		{
			cerr << "Configuration file was corrupted." << endl;
			return 0;
		}
	}

	return 0;
}
