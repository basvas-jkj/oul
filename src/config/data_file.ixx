export module data_file;

import <fstream>;
import <string>;
import <string>;
import <tuple>;
import <vector>;

import <yaml-cpp/yaml.h>;
import <nlohmann/json.hpp>;

import usings;
import message;

using namespace std;
using namespace YAML;
using namespace nlohmann;

using enum CONFIG_ERR;

static bool check_scalar(cr<Node> node, CONFIG_ERR error)
{
	if (node.IsDefined() && node.IsScalar())
	{
		return true;
	}
	else
	{
		report_error(configuration_errors.at(error));
		return false;
	}
}
static bool check_sequence(cr<Node> node, CONFIG_ERR error)
{
	if (node.IsDefined() && node.IsSequence())
	{
		return true;
	}
	else
	{
		report_error(configuration_errors.at(error));
		return false;
	}
}
static bool check_map(cr<Node> node, CONFIG_ERR error)
{
	if (node.IsDefined() && node.IsMap())
	{
		return true;
	}
	else
	{
		report_error(configuration_errors.at(error));
		return false;
	}
}

static bool check_optional_scalar(cr<Node> node, CONFIG_ERR error)
{
	return !node.IsDefined() || check_scalar(node, error);
}
static bool check_optional_sequence(cr<Node> node, CONFIG_ERR error)
{
	return !node.IsDefined() || check_sequence(node, error);
}
static bool check_optional_map(cr<Node> node, CONFIG_ERR error)
{
	return !node.IsDefined() || check_map(node, error);
}

static bool check_file_map(cr<Node> file_map, bool required, CONFIG_ERR error)
{
	bool ok = true;
	if (required)
	{
		ok = check_map(file_map, error);
	}
	else if (file_map.IsDefined())
	{
		ok = check_optional_map(file_map, error);
	}
	else
	{
		return true;
	}

	if (!ok)
	{
		report_error(configuration_errors.at(error));
		return false;
	}
	
	for (cr<pair<Node, Node>> p : file_map)
	{
		cr<Node> file_list = p.second;
		if (!check_sequence(file_list, error))
		{
			ok = false;
			continue;
		}

		for (cr<Node> file : file_list)
		{
			ok &= check_scalar(file, error);
		}
	}
	return ok;
}
static bool check_components_list(cr<Node> list)
{
	if (!check_sequence(list, components_not_array))
	{
		return false;
	}
	for (cr<Node> component : list)
	{
		if (!check_map(component, invalid_component))
		{
			continue;
		}

		bool ok = true;
		ok &= check_scalar(component["name"], missing_component_name);
		ok &= check_optional_scalar(component["original_name"], invalid_original_name);
		ok &= check_scalar(component["location"], missing_location);
		ok &= check_file_map(component["include"], true, missing_include);
		ok &= check_file_map(component["exclude"], false, invalid_exclude);

	}
}

export namespace oul
{
	Node load(cr<string> config_file)
	{
		Node root = LoadFile(config_file);
		if (!check_map(root, root_not_object))
		{
			return Node();
		}
		
		bool ok = true;
		ok &= check_scalar(root["project_name"], missing_project_name);
		ok &= check_scalar(root["default_url"], url_not_string);
		ok &= check_components_list(root["components"]);
		if (ok)
		{
			return root;
		}
		else
		{
			return Node();
		}
	}
	void save(cr<string> location, cr<ordered_json> root)
	{
		ofstream o(location);
		o << root.dump(4);
	}
	void save(cr<string> location, cr<Node> root)
	{
		ofstream o(location);
		o << root;
	}
}
