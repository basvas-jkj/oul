module;

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

export module data_file;

import <fstream>;
import <string>;
import <string>;
import <tuple>;
import <vector>;

import usings;
import message;

using namespace std;
using namespace YAML;
using namespace nlohmann;

using enum CONFIG_ERR;

static bool check_scalar(cr<Node> node, CONFIG_ERR error)
{
	if (node.IsScalar())
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
	if (node.IsSequence())
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
	if (node.IsMap())
	{
		return true;
	}
	else
	{
		report_error(configuration_errors.at(error));
		return false;
	}
}

export namespace oul
{
	Node load(cr<string> config_file)
	{
		vector<string> errors;
		Node root = LoadFile(config_file);
		if (!check_map(root, root_not_object))
		{
			return Node();
		}
		
		bool error = false;
		error |= check_scalar(root["project_name"], missing_project_name);
		error |= check_scalar(root["default_url"], url_not_string);
		error |= check_sequence(root["components"], components_not_array);
		
		if (error)
		{
			return Node();
		}
		else
		{
			return root;
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
