#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
#include <libzippp/libzippp.h>
#include <boost/algorithm/string.hpp>

#include "config.hpp"

using namespace std;
using namespace std::filesystem;

namespace oul
{
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

#pragma region JSON
	optional<CONFIG> read_json(const string& config_file)
	{
		using namespace nlohmann;

		ifstream f(config_file);
		json root = json::parse(f);
		CONFIG cfg;

		json name = root["metadata"]["name"];
		if (name.is_string())
		{
			cfg.name = name.get<string>();
		}
		else
		{
			return nullopt;
		}

		json components = root["components"];
		if (components.is_array())
		{
			for (auto&& c : components)
			{
				if (c["content"].is_array() && c["repository"]["url"].is_string())
				{
					cfg.components.push_back({
						.url = c["repository"]["url"].get<string>(),
						.content = c["content"].get<vector<string>>()
						});
				}
			}
		}

		return cfg;

	}
	void create_json_config(const string& name, const string& default_url)
	{
		using namespace nlohmann;
		json root;
		root["metadata"]["name"] = name;
		root["metadata"]["default_url"] = name;

		ofstream o("oul.config.json");
		o << root;

		clog << "The oul.config.json configuration file was created." << endl;
	}
	void add_component_json(const string& config, const string& url, const vector<string> content)
	{
		using namespace nlohmann;
		ifstream f(config);
		json root = json::parse(f);
		json component;

		for (const string& s : content)
		{
			component["content"].push_back(s);

		}

		component["repository"]["url"] = url;
		root["components"].push_back(component);

		ofstream output(config);
		output << root;
	}
#pragma endregion
#pragma region YAML
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
	void create_yaml_config(const string& name, const string& default_url)
	{
		using namespace YAML;
		Node root;
		root["metadata"]["name"] = name;
		root["metadata"]["default_url"] = default_url;

		ofstream o("oul.config.yaml");
		o << root;

		clog << "The oul.config.yaml configuration file was created." << endl;
	}
	void add_component_yaml(const string& config, const string& url, const vector<string> &content)
	{
		using namespace YAML;
		Node root = LoadFile(config);
		Node component;

		for (const string& s : content)
		{
			component["content"].push_back(s);

		}

		component["repository"]["url"] = url;
		root["components"].push_back(component);

		ofstream output(config);
		output << root;
	}
#pragma endregion

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
			string default_url;
			cout << "Enter the project name: ";
			cin >> name;
			cout << "Do you prefer JSON or YAML for the configuration? (j/y): ";
			cin >> format;
			cout << "Enter URL used by default during installing components: ";
			cin >> default_url;
			
			boost::algorithm::to_lower(format);
			if (format == "j" || format == "json")
			{
				create_json_config(name, default_url);
			}
			else
			{
				create_yaml_config(name, default_url);
			}
		}
		else
		{
			cerr << "Configuration file was already created." << endl;
		}
	}
	void add_component(const string& url, const string& zip_component, const vector<string>& content)
	{
		string config = find_configaration();

		if (config.ends_with(".json"))
		{
			add_component_json(config, url, content);
		}
		else
		{
			add_component_yaml(config, url, content);
		}
		
	}
}