#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string.hpp>

#include "config.hpp"

using namespace std;
using namespace std::filesystem;

namespace oul
{
	optional<CONFIG> read_json(const string& config_file)
	{
		throw exception("coming soon");
	}
	optional<CONFIG> read_yaml(const string& config_file)
	{
		using namespace YAML;
		Node root = LoadFile(config_file);
		CONFIG cfg;

		Node name = root["metadata"]["name"];
		if (name)
		{
			cfg.name = name.as<string>();
		}
		else
		{
			return nullopt;
		}

		Node components = root["components"];
		if (components.IsSequence())
		{
			for (auto&& c : components)
			{
				if (c["content"] && c["repository"]["url"])
				{
					cfg.components.push_back({
						.url = c["repository"]["url"].as<string>(),
						.content = c["content"].as<vector<string>>()
					});
				}
			}
		}

		return cfg;
	}
	void create_json_config(const std::string& name)
	{
		throw exception("coming soon");
	}
	void create_yaml_config(const std::string& name)
	{
		using namespace YAML;
		Node root;
		root["metadata"]["name"] = name;

		ofstream o("oul.config.yaml");
		o << root;

		clog << "The oul.config.yaml configuration file was created." << endl;
	}

	string find_configaration()
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
	optional<CONFIG> read_configuration(const string& config_file)
	{
		if (config_file.ends_with(".json"))
		{
			return read_json(config_file);
		}
		else
		{
			return read_yaml(config_file);
		}
	}
	void initialize()
	{
		if (find_configaration() == "")
		{
			string name;
			string format;
			cout << "Enter the project name: ";
			cin >> name;
			cout << "Do you prefer JSON or YAML for the configuration? (j/y): ";
			cin >> format;
			boost::algorithm::to_lower(format);
			if (format == "j" || format == "json")
			{
				create_json_config(name);
			}
			else
			{
				create_yaml_config(name);
			}
		}
		else
		{
			cerr << "Configuration file was already created." << endl;
		}
	}
}