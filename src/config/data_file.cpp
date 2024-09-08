#include <string>
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

#include "../tmp.hpp"
#include "data_file.hpp"

using namespace std;
using namespace YAML;
using namespace nlohmann;
using namespace boost::filesystem;

namespace oul
{
    /** @brief Zkontroluje platnost uzlu.
     *  @param n kontrolovaný uzel
     *  @param sequence  Má být uzel posloupnost nebo skalární hodnota?
     *  @return Uzel je definovaný a je to platná posloupnost / skalární hodnota.
     **/
    static bool is_valid_node(const Node& n, bool sequence)
    {
        return n.IsDefined() && ((sequence) ? n.IsSequence() : n.IsScalar());
    }

    // -----------------------------
    //      READ COMPONENT FILE
    // -----------------------------

    /** @brief Zkontroluje, zda uzel představuje platnou komponentu.
     *  @param c kontrolovaný uzel
     *  @return Uzel obsahuje skalární atribut „name“ a sekvenční atributy „documantation“, „source_files“ a „test_files“.
     **/
    static bool is_valid_component(const Node& c)
    {
        return is_valid_node(c["name"], false)
            && is_valid_node(c["documentation"], true)
            && is_valid_node(c["source_files"], true)
            && is_valid_node(c["test_files"], true);
    }
    /**
     * @brief Převede uzel na komponentu.
     * @param c uzel převáděný na komponentu
     * @return Pokud je uzel komponenta, vrátí <code>true</code> a přečtenou komponentu. Jinak vrátí <code>false</code>.
     **/
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
                component.original_name = c["repository"]["original_name"].as<string>();
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

    /**
     * @brief Zkontroluje, zda uzel představuje platnou konfiguraci.
     * @param root kontrolovaný uzel
     * @return Uzel obsahuje složený atribut „metadata“ se skalárním atributem „name“.
     **/
    static bool is_valid_config(const Node& root)
    {
        return is_valid_node(root["metadata"]["name"], false);
    }
    /**
     * @brief Převede uzel na konfiguraci.
     * @param c uzel převáděný na konfiguraci
     * @return Je-li konfigurace platná, vrátí <code>true</code> a načtenou konfiguraci. Jinak vrátí <code>false</code>.
     **/
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

    /**
     * @brief Převede informace o komponentě na uzel formátu JSON.
     * @param i objekt nesoucí informace o komponentě
     * @return převedený uzel formátu JSON
     **/
    static ordered_json write_component_json(const ITEM& i)
    {
        ordered_json component;
        component["name"] = i.name;
        component["location"] = i.location;
        component["repository"]["url"] = i.url;
        component["repository"]["original_name"] = i.original_name;
        component["source_files"] = json::array();
        component["test_files"] = json::array();
        component["documentation"] = json::array();

        for (const string& s : i.source_files)
        {
            component["source_files"].push_back(s);
        }
        for (const string& s : i.test_files)
        {
            component["test_files"].push_back(s);
        }
        for (const string& s : i.documentation)
        {
            component["documentation"].push_back(s);
        }

        return component;
    }
    /**
     * @brief Zapíše konfiguraci do souboru ve formátu JSON.
     * @param cfg objekt nesoucí zapisovanou konfiguraci
     **/
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
    /**
     * @brief Převede informace o komponentě na uzel formátu YAML.
     * @param i objekt nesoucí informace o komponentě
     * @return převedený uzel formátu YAML
     **/
    static Node write_component_yaml(const ITEM& i)
    {
        Node component;
        component["name"] = i.name;
        component["location"] = i.location;
        component["repository"]["url"] = i.url;
        component["repository"]["original_name"] = i.original_name;
        component["source_files"] = YAML::Load("[]");
        component["test_files"] = YAML::Load("[]");
        component["documentation"] = YAML::Load("[]");

        for (const string& s : i.source_files)
        {
            component["source_files"].push_back(s);
        }
        for (const string& s : i.test_files)
        {
            component["test_files"].push_back(s);
        }
        for (const string& s : i.documentation)
        {
            component["documentation"].push_back(s);
        }

        return component;
    }
    /**
     * @brief Zapíše konfiguraci do souboru ve formátu YAML.
     * @param cfg objekt nesoucí zapisovanou konfiguraci
     **/
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
    void write_component(const ITEM& i, const string& format)
    {
        path component_config_location = get_temporary_folder(true);
        if (format == "json")
        {
            component_config_location /= "oul.component.json";
            ordered_json component = write_component_json(i);
            ofstream o(component_config_location.string(), ios::noreplace);
            o << component.dump(4);
        }
        else
        {
            component_config_location /= "oul.component.yaml";
            Node component = write_component_yaml(i);
            ofstream o(component_config_location.string(), ios::noreplace);
            o << component;
        }
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
