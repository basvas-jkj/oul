module;

#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

module configuration;

using namespace YAML;
using namespace nlohmann;

namespace oul
{
	/// @brief Ověří, zda uzel obsahuje skalární hodnoru.
	/// @param node - ověřovaný uzel
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel není definován nebo neobsahuje skalární
	/// hodnotu.
	static void check_scalar(cr<Node> node, ERROR name)
	{
		if (!node.IsDefined() || !node.IsScalar())
		{
			throw InvalidConfiguration(name);
		}
	}
	/// @brief Ověří, zda uzel obsahuje posloupnost hodnot.
	/// @param node - ověřovaný uzel
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel není definován nebo neobsahuje
	/// posloupnost hodnot.
	static void check_sequence(cr<Node> node, ERROR name)
	{
		if (!node.IsDefined() || !node.IsSequence())
		{
			throw InvalidConfiguration(name);
		}
	}
	/// @brief Ověří, zda uzel obsahuje mapu.
	/// @param node - ověřovaný uzel
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel není definován nebo neobsahuje mapu.
	static void check_map(cr<Node> node, ERROR name)
	{
		if (!node.IsDefined() || !node.IsMap())
		{
			throw InvalidConfiguration(name);
		}
	}

	/// @brief Ověří, zda uzel obsahuje skalární hodnoru nebo není definovaný.
	/// @param node - ověřovaný uzel
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel je definován a neobsahuje skalární
	/// hodnotu.
	static void check_optional_scalar(cr<Node> node, ERROR name)
	{
		if (node.IsDefined())
		{
			check_scalar(node, name);
		}
	}
	/// @brief Ověří, zda uzel obsahuje posloupnost hodnot nebo není definovaný.
	/// @param node - ověřovaný uzel
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel je definován a neobsahuje posloupnost
	/// hodnot.
	[[maybe_unused]]
	static void check_optional_sequence(cr<Node> node, ERROR name)
	{
		if (node.IsDefined())
		{
			check_sequence(node, name);
		}
	}
	/// @brief Ověří, zda uzel obsahuje mapu nebo není definovaný.
	/// @param node - ověřovaný uzel
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel je definován a neobsahuje mapu.
	static void check_optional_map(cr<Node> node, ERROR name)
	{
		if (node.IsDefined())
		{
			check_map(node, name);
		}
	}

	/// @brief Zkontroluje, zda uzel obsahuje kategorizovaný seznam souborů.
	/// Tedy zda jde o mapu obsahující posloupnosti skalárních hodnot.
	/// @param file_map - kontrolovaný uzel
	/// @param required - vyžaduje se, aby byl uzel definován?
	/// @param name - jméno chyby, kterou má obsahovat vyhozená výjimka
	/// @throw <code>InvalidConfiguration</code>, pokud uzel nereprezentuje kategorizovaný seznam
	/// souborů.
	static void check_file_map(cr<Node> file_map, bool required, ERROR name)
	{
		if (required)
		{
			check_map(file_map, name);
		}
		else if (file_map.IsDefined())
		{
			check_optional_map(file_map, name);
		}
		else
		{
			return;
		}

		for (cr<pair<Node, Node>> p : file_map)
		{
			cr<Node> file_list = p.second;
			check_sequence(file_list, name);

			for (cr<Node> file : file_list)
			{
				check_scalar(file, name);
			}
		}
	}
	/// @brief Zkontroluje, zda uzel obsahuje platnou konfiguraci komponenty.
	/// @param component - kontrolovaný uzel
	/// @param validate_location - Je součástí kontroly atribut <code>location</code>?
	/// @throw <code>InvalidConfiguration</code>, pokud uzel neukládá platnou konfiguraci
	/// komponenty.
	static void check_component(cr<Node> component, bool validate_location)
	{
		check_map(component, ERROR::invalid_component);

		check_scalar(component["name"], ERROR::invalid_component_name);
		check_optional_scalar(component["original_name"], ERROR::invalid_original_name);
		check_file_map(component["include"], true, ERROR::missing_include);
		check_file_map(component["exclude"], false, ERROR::invalid_exclude);

		if (validate_location)
		{
			check_scalar(component["location"], ERROR::missing_location);
		}
	}
	/// @brief Zkontroluje, zda uzel obsahuje posloupnost konfigirací komponenty.
	/// @param list - kontrolovaný uzel
	/// @throw <code>InvalidConfiguration</code>, pokud uzel není definovaný nebo neukládá
	/// posloupnost konfigurací komponenty.
	static void check_components_list(cr<Node> list)
	{
		check_sequence(list, ERROR::components_not_array);

		for (cr<Node> component : list)
		{
			check_component(component, true);
		}
	}

	/// @brief Načte a zkontroluje konfiguraci ze souboru
	/// @param config_file - cesta ke konfiguračnímu souboru
	/// @return platná konfigurace
	/// @throw <code>InvalidConfiguration</code>, pokud soubor neukládá platnou konfiguraci.
	Node load(istream& config_file)
	{
		try
		{
			Node root = Load(config_file);

			check_map(root, ERROR::root_not_object);
			check_scalar(root["project_name"], ERROR::missing_project_name);
			check_scalar(root["default_url"], ERROR::url_not_string);
			check_components_list(root["components"]);

			return root;
		}
		catch (ParserException&)
		{
			throw InvalidConfiguration(ERROR::invalid_configuration_format);
		}
	}
	/// @brief Načte a zkontroluje konfiguraci komponenty ze souboru
	/// @param component_file - cesta ke konfiguračnímu souboru komponenty
	/// @param validate_location - vyžaduje se přítomnost atributu <code>location</code>?
	/// @return platná konfigurace komponenty
	/// @throw <code>InvalidConfiguration</code>, pokud soubor neukládá platnou konfiguraci
	/// komponenty.
	Node load_component(istream& component_file, bool validate_location)
	{
		try
		{
			Node root = Load(component_file);
			check_component(root, validate_location);
			return root;
		}
		catch (ParserException&)
		{
			throw InvalidConfiguration(ERROR::invalid_configuration_format);
		}
	}

	/// @brief Převede konfiguraci komponenty do formátu JSON.
	/// @param i - konfigurace komponenty
	/// @return konfigurace komponenty ve formátu JSON
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
	/// @brief Převede konfiguraci projektu do formátu JSON.
	/// @param c - konfigurace projektu
	/// @return konfigurace projektu ve formátu JSON
	static ordered_json genererate_json(cr<CONFIG> c)
	{
		ordered_json root;

		root.emplace("project_name", c.project_name);
		root.emplace("default_url", c.default_url);
		root.emplace("components", "[]"_json);

		for (cr<ITEM> i : c.components)
		{
			ordered_json component = genererate_json(i);
			root["components"].push_back(std::move(component));
		}

		return root;
	}
	/// @brief Převede konfiguraci komponenty do formátu YAML.
	/// @param i - konfigurace komponenty
	/// @return konfigurace komponenty ve formátu YAML
	static Node genererate_yaml(cr<ITEM> i)
	{
		Node component;

		component["name"] = i.name;
		component["original_name"] = i.original_name;
		component["url"] = i.url;
		component["location"] = i.location;
		component["include"] = i.include;
		component["exclude"] = i.exclude;

		return component;
	}
	/// @brief Převede konfiguraci projektu do formátu YAML.
	/// @param c - konfigurace projektu
	/// @return konfigurace projektu ve formátu YAML
	static Node genererate_yaml(cr<CONFIG> c)
	{
		Node root;

		root["project_name"] = c.project_name;
		root["default_url"] = c.default_url;

		for (cr<ITEM> i : c.components)
		{
			Node component = genererate_yaml(i);
			root["components"].push_back(std::move(component));
		}

		if (!root["components"].IsSequence())
		{
			root["components"] = Load("[]");
		}

		return root;
	}

	/// @brief Uloží konfiguraci projektu ve formátu JSON.
	/// @param c - konfigurace projektu
	void save_json(cr<CONFIG> c)
	{
		ordered_json root = genererate_json(c);
		ofstream o(c.location);
		o << root.dump(4);
	}
	/// @brief Uloží konfiguraci komponenty ve formátu YAML.
	/// @param c - konfigurace projektu
	void save_yaml(cr<CONFIG> c)
	{
		Node root = genererate_yaml(c);
		ofstream o(c.location);
		o << root;
	}

	/// @brief Uloží konfiguraci komponenty ve formátu JSON.
	/// @param c - konfigurace komponenty
	/// @param output - stream, do kterého bude konfigurace zapsána
	void save_json(cr<ITEM> c, ofstream& output)
	{
		output << genererate_json(c).dump(4);
	}
	/// @brief Uloží konfiguraci komponenty ve formátu YAML.
	/// @param c - konfigurace komponenty
	/// @param output - stream, do kterého bude konfigurace zapsána
	void save_yaml(cr<ITEM> c, ofstream& output)
	{
		output << genererate_yaml(c);
	}
}
