module;

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

export module config:data_file;

import usings;

using namespace std;
using namespace YAML;
using namespace nlohmann;

namespace oul
{
    export struct ITEM;
    export struct CONFIG;

    export Node load(cr<string> config_file);
    export Node load_component(cr<string> component_file);
    export void save_json(cr<CONFIG> c);
    export void save_yaml(cr<CONFIG> c);

    export void save_json(cr<ITEM> c, ofstream& output);
    export void save_yaml(cr<ITEM> c, ofstream& output);
}
