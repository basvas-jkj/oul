module;

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

module config;

using namespace YAML;
using namespace nlohmann;
using enum CONFIG_ERR;

namespace oul
{
	static void check_scalar(cr<Node> node, CONFIG_ERR error)
	{
		if (!node.IsDefined() || !node.IsScalar())
		{
			throw InvalidConfiguration(configuration_errors.at(error));
		}
	}
	static void check_sequence(cr<Node> node, CONFIG_ERR error)
	{
		if (!node.IsDefined() || !node.IsSequence())
		{
			throw InvalidConfiguration(configuration_errors.at(error));
		}
	}
	static void check_map(cr<Node> node, CONFIG_ERR error)
	{
		if (!node.IsDefined() || !node.IsMap())
		{
			throw InvalidConfiguration(configuration_errors.at(error));
		}
	}

	static void check_optional_scalar(cr<Node> node, CONFIG_ERR error)
	{
		if (node.IsDefined())
		{
			check_scalar(node, error);
		}
	}
	static void check_optional_sequence(cr<Node> node, CONFIG_ERR error)
	{
		if (node.IsDefined())
		{
			check_sequence(node, error);
		}
	}
	static void check_optional_map(cr<Node> node, CONFIG_ERR error)
	{
		if (node.IsDefined())
		{
			check_map(node, error);
		}
	}

	static void check_file_map(cr<Node> file_map, bool required, CONFIG_ERR error)
	{
		if (required)
		{
			check_map(file_map, error);
		}
		else if (file_map.IsDefined())
		{
			check_optional_map(file_map, error);
		}
		else
		{
			return;
		}

		for (cr<pair<Node, Node>> p : file_map)
		{
			cr<Node> file_list = p.second;
			check_sequence(file_list, error);

			for (cr<Node> file : file_list)
			{
				check_scalar(file, error);
			}
		}
	}
	static void check_components_list(cr<Node> list)
	{
		check_sequence(list, components_not_array);

		for (cr<Node> component : list)
		{
			check_map(component, invalid_component);

			check_scalar(component["name"], missing_component_name);
			check_optional_scalar(component["original_name"], invalid_original_name);
			check_scalar(component["location"], missing_location);
			check_file_map(component["include"], true, missing_include);
			check_file_map(component["exclude"], false, invalid_exclude);
		}
	}
	
	/**
	 * @brief Načte a zkontroluje konfiguraci ze souboru
	 * @param config_file - cesta ke konfiguračnímu souboru
	 * @return platná konfigurace
	 * @throw InvalidConfiguration
	 **/
	Node load(cr<string> config_file)
	{
		Node root = LoadFile(config_file);

		check_map(root, root_not_object);
		check_scalar(root["project_name"], missing_project_name);
		check_scalar(root["default_url"], url_not_string);
		check_components_list(root["components"]);

		return root;
	}

	/**
	 * @brief Převede konfiguraci komponenty do formátu JSON.
	 * @return konfigurace komponenty ve formátu JSON
	 **/
	static ordered_json genererate_json(cr<ITEM> i)
	{
		ordered_json component;

		component.emplace("name", i.name);
		component.emplace("original_name", i.original_name);
		component.emplace("url", i.url);
		component.emplace("location", i.location);
		component.emplace("include", i.include);
		component.emplace("exclude", i.exclude);

		return component;
	}
	/**
	 * @brief Uloží konfiguraci ve formátu JSON.
	 * @return konfigurace projektu ve formátu JSON
	 **/
	static ordered_json genererate_json(cr<CONFIG> c)
	{
		ordered_json root;

		root.emplace("project_name", c.project_name);
		root.emplace("default_url", c.default_url);
		root.emplace("components", "[]"_json);

		for (cr<ITEM> i : c.components)
		{
			ordered_json component = genererate_json(i);
			root["components"].push_back(move(component));
		}

		return root;
	}
	/**
	 * @brief Převede konfiguraci komponenty do formátu YAML.
	 * @return konfigurace komponenty ve formátu YAML
	 **/
	static Node genererate_yaml(cr<ITEM> i)
	{
		Node component;

		component["name"] = i.name;
		component["original_name"] = i.original_name;
		component["url"] = i.url;
		component["location"] = i.location;;
		component["include"] = i.include;
		component["exclude"] = i.exclude;

		return component;
	}
	/**
	 * @brief Uloží konfiguraci ve formátu YAML.
	 * @return konfigurace projektu ve formátu YAML
	 **/
	static Node genererate_yaml(cr<CONFIG> c)
	{
		Node root;

		root["project_name"] = c.project_name;
		root["default_url"] = c.default_url;

		for (cr<ITEM> i : c.components)
		{
			Node component = genererate_yaml(i);
			root["components"].push_back(move(component));
		}

		if (!root["components"].IsSequence())
		{
			root["components"] = Load("[]");
		}

		return root;
	}

	/**
	 * @brief Uloží konfiguraci ve formátu JSON.
	 * @param location - cesta ke konfiguračnímu souboru
	 * @param root - objekt konfigurace
	 **/
	void save_json(cr<CONFIG> c)
	{
		ordered_json root = genererate_json(c);

		ofstream o(c.location);
		o << root.dump(4);
	}
	/**
	 * @brief Uloží konfiguraci ve formátu YAML.
	 * @param location - cesta ke konfiguračnímu souboru
	 * @param root - objekt konfigurace
	 **/
	void save_yaml(cr<CONFIG> c)
	{
		Node root = genererate_yaml(c);

		ofstream o(c.location);
		o << root;
	}
}
