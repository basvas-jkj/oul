module;

#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

export module configuration:data_file;

import item;
import support;

using namespace std;
using namespace YAML;
using namespace nlohmann;

namespace oul
{
	export struct CONFIG;

	export Node load(istream& config_file);
	export Node load_component(istream& component_file, bool validate_location = true);
	export void save_json(cr<CONFIG> c);
	export void save_yaml(cr<CONFIG> c);

	export void save_json(cr<ITEM> c, ofstream& output);
	export void save_yaml(cr<ITEM> c, ofstream& output);
}
