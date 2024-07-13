#include <fstream>
#include <iostream>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
#include <libzippp/libzippp.h>
#include <boost/algorithm/string.hpp>

#include "config.hpp"
#include "data_file/data_file.hpp"

using namespace std;
using namespace std::filesystem;

namespace oul
{
    // -------------------
    //       CONFIG        
    // -------------------
    string CONFIG::find()
    {
        path current = current_path();
        do
        {
            path parent = current.parent_path();
            current.append("oul.config.json");

            if (exists(current))
            {
                return current.string();
            }
            
            current.replace_filename("oul.config.yaml");
            if (exists(current))
            {
                return current.string();
            }

            current = parent;
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
            write_config(cfg);
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
        write_config(*this);
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

    string CONFIG::get_url(const string& component_name) const
    {
        return default_url + "/" + component_name;
    }
}