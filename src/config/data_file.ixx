module;

#include <yaml-cpp/yaml.h>

export module config:data_file;

import std;
import usings;

using namespace std;
using namespace YAML;

namespace oul
{
	class CONFIG;

	export Node load(cr<string> config_file);
	export void save_json(cr<CONFIG> c);
	export void save_yaml(cr<CONFIG> c);
}
