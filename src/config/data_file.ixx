module;

#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

export module config:data_file;

import item;
import usings;

using namespace std;
using namespace YAML;
using namespace nlohmann;

namespace oul
{
	export struct CONFIG;

	export Node load(cr<string> config_file);
	export Node load_component(cr<string> component_file, bool validate_location = true);
	export void save_json(cr<CONFIG> c);
	export void save_yaml(cr<CONFIG> c);

	export void save_json(cr<ITEM> c, ofstream& output);
	export void save_yaml(cr<ITEM> c, ofstream& output);
}
