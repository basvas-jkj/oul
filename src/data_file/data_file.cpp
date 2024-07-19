#include <string>
#include <fstream>

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

#include "data_file.hpp"

using namespace std;
using namespace YAML;
using namespace nlohmann;

namespace oul
{
    // -----------------------------
    //      READ COMPONENT FILE
    // -----------------------------
    static bool is_valid_component(const Node& c)
    {
        return c["name"].IsScalar() && c["documentation"].IsSequence()
            && c["source_files"].IsSequence() && c["test_files"].IsSequence();
    }
    static optional<ITEM> read_component(const Node& c)
    {
        ITEM component;

        if (is_valid_component(c))
        {
            component.name = c["name"].as<string>();
            component.documentation = c["documentation"].as<vector<string>>();
            component.source_files = c["source_files"].as<vector<string>>();
            component.test_files = c["test_files"].as<vector<string>>();

            if (c["repository"]["url"].IsScalar())
            {
                component.url = c["repository"]["url"].as<string>();
            }

            return {true, component};
        }
        else
        {
            return {false, component};
        }
    }
    optional<ITEM> read_component(const string& s)
    {
        Node c = Load(s);
        return read_component(c);
    }

    // -----------------------------
    //       READ CONFIG FILE
    // -----------------------------
    static bool is_valid_config(const Node& root)
    {
        return root["metadata"]["name"].IsScalar();
    }
    static optional<CONFIG> read_config(CONFIG& cfg,  const Node& root)
    {
        if (is_valid_config(root))
        {
            cfg.name = root["metadata"]["name"].as<string>();

            if (root["metadata"]["default_url"].IsScalar())
            {
                cfg.default_url = root["metadata"]["default_url"].as<string>();
            }

            Node components = root["components"];
            if (components.IsDefined() && components.IsSequence())
            {
                int f = 0;
                while (components[f].IsDefined())
                {
                    auto [valid, component] = read_component(components[f]);
                    if (valid)
                    {
                        cfg.components.push_back(component);
                    }
                    f += 1;
                }
            }

            return {true, cfg};
        }
        else
        {
            return {false, cfg};
        }

        
    }
	optional<CONFIG> load_config(const string& config_file)
	{
        CONFIG cfg(config_file);
        Node root = LoadFile(config_file);

        return read_config(cfg, root);
	}

    // -----------------------------
    //       WRITE CONFIG FILE
    // -----------------------------
    static ordered_json write_component_json(const ITEM& i)
    {
        ordered_json component;
        component["name"] = i.name;
        component["repository"]["url"] = i.url;

        for (const string& s : i.source_files)
        {
            component["source_files"].push_back(s);
        }
        for (const string& s : i.source_files)
        {
            component["test_files"].push_back(s);
        }
        for (const string& s : i.documentation)
        {
            component["documentation"].push_back(s);
        }

        return component;
    }
    static void write_config_json(const CONFIG& cfg)
    {
        ordered_json root;
        root["metadata"]["name"] = cfg.name;
        root["metadata"]["default_url"] = cfg.default_url;

        for (const ITEM& i : cfg.components)
        {
            ordered_json component = write_component_json(i);
            root["components"].push_back(component);
        }

        ofstream o(cfg.location);
        o << root.dump(4);
    }
    static Node write_component_yaml(const ITEM& i)
    {
        Node component;
        component["name"] = i.name;
        component["repository"]["url"] = i.url;

        for (const string& s : i.source_files)
        {
            component["source_files"].push_back(s);
        }
        for (const string& s : i.source_files)
        {
            component["test_files"].push_back(s);
        }
        for (const string& s : i.documentation)
        {
            component["documentation"].push_back(s);
        }

        return component;
    }
    static void write_config_yaml(const CONFIG& cfg)
    {
        Node root;
        root["metadata"]["name"] = cfg.name;
        root["metadata"]["default_url"] = cfg.default_url;

        for (const ITEM& i : cfg.components)
        {
            Node component = write_component_yaml(i);
            root["components"].push_back(component);
        }

        ofstream o(cfg.location);
        o << root;
    }
    void write_config(const CONFIG& cfg)
    {
        if (cfg.location.ends_with(".json"))
        {
            write_config_json(cfg);
        }
        else
        {
            write_config_yaml(cfg);
        }
    }
}
