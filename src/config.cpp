#include <optional>
#include <filesystem>
#include <yaml-cpp/yaml.h>

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
}