module;

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <yaml-cpp/yaml.h>

export module configuration;

import support;
import message;
export import item;
export import :data_file;

using namespace std;
using boost::algorithm::to_lower;
using boost::algorithm::trim;
namespace fs = boost::filesystem;

namespace oul
{
	/// @brief Chyba: neplatná konfigurace.
	export class InvalidConfiguration: public CommonException
	{
	public:
		InvalidConfiguration(MESSAGE name): CommonException(name) {}
	};

	/// @brief Struktura pro ukládání konfigurace projektu.
	export struct CONFIG
	{
		string location;
		string project_name;
		string default_url;
		vector<ITEM> components;

		/// @brief Konstruktor vytvářející prázdný objekt structury <code>CONFIG</code>.
		CONFIG() {}
		/// @brief Konstruktor vytvářející objekt structury <code>CONFIG</code>.
		/// Současně také načte konfiguraci ze souboru a zvaliduje.
		/// @param l - umístění konfiguračního souboru v souborovém systému
		CONFIG(cr<string> l): CONFIG(ifstream(l))
		{
			location = l;
		}
		CONFIG(derived_from<istream> auto&& source)
		{
			Node root = load(source);

			project_name = root["project_name"].as<string>();
			default_url = root["default_url"].as<string>();

			for (cr<Node> component : root["components"])
			{
				components.emplace_back(component);
			}
		}
		CONFIG(cr<CONFIG>) = delete;
		CONFIG(CONFIG&&) = default;
		/// @brief Zajišťuje zápsání změněné konfigurace do souboru.
		~CONFIG() noexcept
		{
			try
			{
				if (location.ends_with(".json"))
				{
					save_json(*this);
				}
				else if (location.ends_with(".yaml"))
				{
					save_yaml(*this);
				}
			}
			catch (...)
			{
				report_error(MESSAGE::unexpected_error);
			}
		}

		/// @brief Vytvoří prázdnou konfiguraci. Potřebné údaje získá interaktivně ze standardního
		/// vstupu.
		static void initialize()
		{
			string name;

			print_init_prompt(MESSAGE::init_name);
			getline(cin, name);
			trim(name);

			string format;
			print_init_prompt(MESSAGE::init_format);
			getline(cin, format);
			trim(format);
			to_lower(format);

			if (format != "y" && format != "yaml" && format[0] == 'y')
			{
				report_error(MESSAGE::init_yaml_fallback);
			}
			else
			{
				report_error(MESSAGE::init_json_fallback);
			}

			string default_url;
			while (true)
			{
				print_init_prompt(MESSAGE::init_url);
				getline(cin, default_url);
				trim(default_url);

				if (default_url == "" || validate_url(default_url))
				{
					break;
				}
			}

			string location = (format[0] == 'y') ? "oul.config.yaml" : "oul.config.json";
			location = (fs::current_path() / location).string();

			CONFIG cfg;
			cfg.location = std::move(location);
			cfg.project_name = name;
			cfg.default_url = default_url;
		}
		/// @brief Najde cestu ke konfiguračnímu souboru.Pokud je soubor nalezen, nastaví pracovní
		/// složku na jeho umístění.
		/// @return Pokud byla konfigurace nalezena, vrátí cestu. Jinak vrátí "".
		static std::optional<string> find()
		{
			fs::path current = fs::current_path();
			do
			{
				fs::path config = current / "oul.config.json";
				if (fs::exists(config))
				{
					return config.string();
				}

				config = current / "oul.config.yaml";
				if (fs::exists(config))
				{
					return config.string();
				}

				current = current.parent_path();
			} while (!equivalent(current, current.root_path()));
			return nullopt;
		}

		/// @brief Převede seznam všech komponent v konfiguraci na seznam názvů.
		/// @return iterovatelný seznam (<code>view</code>) názvů všech komponent
		auto get_component_names() const
		{
			auto item_to_name = [](cr<ITEM> i) { return i.name; };
			return views::transform(components, item_to_name);
		}
		/// @param i - konfigurace komponenty
		void add_component(cr<ITEM> i)
		{
			components.push_back(i);
		}
		/// @brief Odebere komponentu z konfigurace.
		/// @param name - jméno komponenty
		void remove_component(cr<string> name)
		{
			auto has_equal_name = [&name](ITEM& i) { return i.name == name; };
			erase_if(components, has_equal_name);
		}
		/// @brief Zjistí, zda konfigurace projektu obsahuje komponentu daného jména.
		/// @param name - jméno komponenty
		/// @return <code>true</code>, pokud konfigurace danou komponentu obsahuje,
		/// <code>false</code> v opačném případě.
		bool contains(cr<string> name) const
		{
			for (cr<string> component : get_component_names())
			{
				if (component == name)
				{
					return true;
				}
			}
			return false;
		}
		/// @brief Najde v konfiguraci projektu komponentu a vrátí iterátor.
		/// @param name - jméno komponenty
		/// @return Platný iterátor ukazující na komponentu, pokud je v konfiguraci přítomná. Jinak
		/// vrátí <code>end()</code>
		vector<ITEM>::iterator get_component(cr<string> name)
		{
			return ranges::find_if(components, [name](cr<ITEM> i) { return i.name == name; });
		}

		/// @brief Získá výchozí URL pro stažení komponenty.
		/// @param component_name - jméno komponenty
		/// @return spojení výchozí URL projektu a jména komponenty
		string get_url(cr<string> component_name) const
		{
			return (fs::path(default_url) / component_name).generic_string();
		}
		/// @brief Vrací cestu složky nadřazené konfiguračnímu souboru.
		/// @return cesta ke složce, ve které je uložen konfigurační soubor
		fs::path get_directory() const
		{
			return fs::path(location).parent_path();
		}
	};
}
