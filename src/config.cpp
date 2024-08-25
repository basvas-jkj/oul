#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
#include <libzippp/libzippp.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "config.hpp"
#include "data_file/data_file.hpp"

using namespace std;
using namespace boost::filesystem;

namespace oul
{
    // -------------------
    //       CONFIG        
    // -------------------
    CONFIG::~CONFIG()
    {
        write_config(*this);
    }
    string CONFIG::find()
    {
        path current = current_path();
        do
        {
            path config = current / "oul.config.json";
            if (exists(config))
            {
                current_path(current);
                return config.string();
            }
            
            config = current / "oul.config.yaml";
            if (exists(config))
            {
                current_path(current);
                return config.string();
            }

            current = current.parent_path();
        }
        while (!equivalent(current, current.root_path()));
        return "";
    }
    void CONFIG::initialize()
    {
        if (find() == "")
        {
            string name;
            string format;
            string default_url;
            cout << "Enter the project name: ";
            cin >> name;
            cout << "Do you prefer JSON or YAML for the configuration? (j/y): ";
            cin >> format;
            boost::algorithm::to_lower(format);
            cout << "Enter URL used by default during installing components: ";
            cin >> default_url;
            
            string location = (format.starts_with('j')) ? "oul.config.json" : "oul.config.yaml";
            CONFIG cfg(location);
            cfg.name = name;
            cfg.default_url = default_url;
        }
        else
        {
            cerr << "Configuration file was already created." << endl;
        }
    }
    optional<CONFIG> CONFIG::read()
    {
        return read(find());
    }
    optional<CONFIG> CONFIG::read(const string& config_file)
    {
        return load_config(config_file);
    }
    void CONFIG::add_component(const ITEM& i)
    {
        components.push_back(i);
    }
    void CONFIG::list_components()
    {
        cout << "List of components used in this project:" << endl;
        for (ITEM& i : components)
        {
            const string& url = (i.url.empty()) ? "(local only)" : i.url;
            cout << "    " << i.name << '\t' << url << endl;
        }
    }
    bool CONFIG::contains(const string& name)
    {
        for (const string& component: get_components())
        {
            if (component == name)
            {
                return true;
            }
        }
        return false;
    }
    vector<ITEM>::iterator CONFIG::get_component(const string& name)
    {
        return ranges::find_if(components, [name](ITEM& i){return i.name == name;});
    }

    string CONFIG::get_url(const string& component_name) const
    {
        return default_url + "/" + component_name;
    }
}