export module data_file;

import std;
import usings;
import message;
export import <yaml-cpp/yaml.h>;
export import <nlohmann/json.hpp>;

using namespace std;
using namespace YAML;
using namespace nlohmann;
using enum CONFIG_ERR;

namespace oul
{
	/**
	 * @brief Chyba v konfiguraci.
	 **/
	export class InvalidConfiguration: public exception
	{
		string message;

	public:
		InvalidConfiguration(cr<string> m): message(m)
		{}
		InvalidConfiguration(string&& m): message(m)
		{}
		void report() const
		{
			report_error(message);
		}
	};

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
	 * @brief Naète a zkontroluje konfiguraci ze souboru
	 * @param config_file cesta ke konfiguraènímu souboru
	 * @return platná konfigurace
	 * @throw InvalidConfiguration
	 **/
	export Node load(cr<string> config_file)
	{
		Node root = LoadFile(config_file);

		check_map(root, root_not_object);
		check_scalar(root["project_name"], missing_project_name);
		check_scalar(root["default_url"], url_not_string);
		check_components_list(root["components"]);

		return root;
	}
	/**
	 * @brief Uloží konfiguraci ve formátu JSON.
	 * @param location cesta ke konfiguraènímu souboru
	 * @param root objekt konfigurace
	 **/
	export void save(cr<string> location, cr<ordered_json> root)
	{
		ofstream o(location);
		o << root.dump(4);
	}
	/**
	 * @brief Uloží konfiguraci ve formátu YAML.
	 * @param location cesta ke konfiguraènímu souboru
	 * @param root objekt konfigurace
	 **/
	export void save(cr<string> location, cr<Node> root)
	{
		ofstream o(location);
		o << root;
	}
}
