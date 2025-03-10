module;

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

export module config:data_file;

import std;
import usings;

using namespace std;
using namespace YAML;
using namespace nlohmann;

namespace oul
{
	export struct ITEM;
	export struct CONFIG;

	export Node load(cr<string> config_file);
	export void save_json(cr<CONFIG> c);
	export void save_yaml(cr<CONFIG> c);

	export ordered_json genererate_json(cr<ITEM> i);
	export Node genererate_yaml(cr<ITEM> i);
}
