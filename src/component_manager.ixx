module;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <span>
#include <string>

export module component_manager;

import support;
import server;
import message;
export import config;

using namespace std;
namespace fs = boost::filesystem;

/// @brief Zkontroluje, zda je řetězec vzor nebo konkrétní cesta.
/// @param name - kontrolovaný řetězec
/// @return <code>true</code>, pokud je řetězec vzor cesty, <code>false</code>, pokud jde o konkréní
/// cestu
bool is_pattern(cr<string> name)
{
	vector<char> special_chars{'*', '?', '\\', '[', ']', '(', ')', '|', '!'};
	for (char ch : special_chars)
	{
		if (name.contains(ch))
		{
			return true;
		}
	}
	return false;
}
/// @brief Zkontroluje, zda je zadaná položka platná.
/// @param entry - cesta nebo vzor
/// @return <code>true</code>, pokud cesta odkazuje na existující soubor nebo jde-li o vzor,
/// <code>false</code> v opačném případě
bool check_entry_validity(cr<string> entry)
{
	return is_pattern(entry) || fs::exists(entry);
}
/// @brief Je daná položka mimo složku, která ukládá komponentu?
/// @param entry - cesta k položce relativní vůči umístění komponenty
/// @return <code>true</code>, pokud se položka nachází mimo složku komponenty, jinak
/// <code>false</code>
bool is_outside_component(cr<fs::path> entry)
{
	return entry.string().starts_with("..");
}
/// @brief Přidá řetězec do vektoru, pokud tam doposud nebyl.
/// @param v - vektor, do kterého se řetězec přidává
/// @param s - řetězec přidávaný do vektoru
void add_if_not_presented(vector<string>& v, cr<string> s)
{
	if (ranges::find(v, s) == v.end())
	{
		v.push_back(s);
	}
}

namespace oul
{
	/// @brief Chyba: konfigurace neobsahuje hledanou komponentu.
	export class MissingComponent: public CommonException
	{
	public:
		MissingComponent(ERROR name = ERROR::component_not_found): CommonException(name) {}
	};
	/// @brief Chyba: konfigurace komponenty neobsahuje hledanou skupinu.
	export class MissingGroup: public CommonException
	{
	public:
		MissingGroup(ERROR name): CommonException(name) {}
	};

	/// @brief Třída pro vytváření a správu komponent.
	export class COMPONENT_MANAGER
	{
		CONFIG cfg;

		/// @brief Upraví cestu k souboru na relativní vzhledem k umístění komponenty.
		/// @param file - cesta k souboru
		/// @param component_path - cesta ke komponentě relativní vzhledem k umístění konfiguračního
		/// souboru
		/// @return cesta k souboru relativní vzhledem k umístění komponenty
		fs::path shift(cr<fs::path> file, cr<fs::path> component_path) const
		{
			fs::path cfg_path = fs::path(cfg.location).parent_path();
			return fs::relative(file, cfg_path / component_path);
		}
		/// @brief Najde komponentu v konfiguraci projektu.
		/// @param component - jméno hledané komponenty
		/// @return reference na konfiguraci komponenty, pokud byla nalezena
		/// @throw <code>MissingComponent</code>, pokud komponenta nebyla nalezena
		ITEM& find_component(cr<string> component)
		{
			vector<ITEM>::iterator component_it = cfg.get_component(component);
			if (component_it == cfg.components.end())
			{
				throw MissingComponent(ERROR::component_not_found);
			}
			else
			{
				return *component_it;
			}
		}
		/// @brief Najde a vrátí seznam souborů v zadané skupině ze zadaného seznamu souborů.
		/// @param files - seznam souborů členěný podle kategorií
		/// @param group - jméno hledané skupiny
		/// @return seznam souborů odpovídající dané skupině
		/// @throw <code>MissingGroup</code>, pokud zadaná skupina nalezena nebyla
		vector<string>& find_file_list(file_map& files, cr<string> group)
		{
			file_map::iterator group_it = files.find(group);
			if (group_it == files.end())
			{
				throw MissingGroup(ERROR::group_not_found);
			}
			else
			{
				return group_it->second;
			}
		}

	public:
		/// @brief Konstruktor vytvářející objekty třídy <code>COMPONENT_MANAGER</code>.
		/// @param c - objekt ukládající konfiguraci programu OUL
		COMPONENT_MANAGER(CONFIG&& c): cfg(std::move(c)) {};

		/// @brief Vytvoří novou komponentu v projektu.
		/// @param name - jméno komponenty
		/// @param location - umístění komponenty
		void create(cr<string> name, cr<string> location)
		{
			fs::path cfg_path = fs::path(cfg.location).parent_path();
			fs::path shifted = fs::relative(location, cfg_path);

			ITEM i(name, shifted.generic_string());
			cfg.add_component(i);
		}
		/// @brief Stáhne novou komponentu ze serveru.
		/// @param server_name - jméno komponentu na serveru
		/// @param local_name - jméno, pod jakým bude komponenta uložena v konfiguraci
		/// @param url - url komponenty nebo cesta ke složce
		/// @param where - umístění komponenty
		void install(cr<string> server_name, cr<string> local_name, cr<string> url,
					 cr<string> where)
		{
			if (cfg.contains(local_name))
			{
				report_error(ERROR::component_already_exists);
				return;
			}

			client_ptr client = select_client(url, cfg.get_directory() / where);
			ITEM component = client->download();
			component.name = local_name;
			component.original_name = server_name;
			component.location = where;
			component.url = fs::path(url).parent_path().generic_string();

			cfg.add_component(component);
		}
		/// @brief Nahraje komponentu na server.
		/// @param url - url komponenty nebo cesta ke složce
		/// @param location - umístění komponenty
		/// @param component - komponenta nahrávaná na server
		void upload(cr<string> url, cr<fs::path> location, cr<ITEM> component)
		{
			client_ptr client = select_client(url, location);
			client->upload(component);
		}
		/// @brief Odebere komponentu z projektu.
		/// @param name - jméno komponenty
		void remove(cr<string> name)
		{
			cfg.remove_component(name);
		}
		/// @brief Přejmenuje zadanou komponentu.
		/// @param original_name - původní jméno komponenty
		/// @param new_name - nové jméno komponenty
		void rename(cr<string> original_name, cr<string> new_name)
		{
			if (cfg.contains(new_name))
			{
				report_error(ERROR::component_already_exists);
				return;
			}
			vector<ITEM>::iterator it = cfg.get_component(original_name);
			if (it == cfg.components.end())
			{
				report_error(ERROR::component_not_found);
				return;
			}

			it->name = new_name;
		}

		/// @brief Přidá skupinu souborů do zadané komponenty.
		/// @param name - jméno komponenty
		/// @param group - jméno skupiny
		void add_group(cr<string> name, cr<string> group)
		{
			ITEM& component = find_component(name);
			component.include.insert({group, vector<string>()});
			component.exclude.insert({group, vector<string>()});
		}
		/// @brief Odebere skupinu souborů ze zadané komponenty.
		/// @param name - jméno komponenty
		/// @param group - jméno skupiny
		void remove_group(cr<string> name, cr<string> group)
		{
			ITEM& component = find_component(name);
			component.include.erase(group);
			component.exclude.erase(group);
		}

		/// @brief Přidá do konfigurace zadané komponenty nové soubory (musí existovat) nebo vzory
		/// cest.
		/// @param component - jméno komponenty
		/// @param group - jméno skupiny
		/// @param entries - seznam souborů, složek a vzorů
		void add_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& include_list = find_file_list(component.include, group);

			for (cr<string> entry : entries)
			{
				fs::path shifted = shift(entry, component.location);
				if (is_outside_component(shifted))
				{
					report_error(ERROR::file_outside_component, {name, component.location});
				}
				else if (check_entry_validity(entry))
				{
					add_if_not_presented(include_list, shifted.generic_string());
				}
				else
				{
					report_error(ERROR::file_not_exist);
				}
			}
		}
		/// @brief Vyčlení z komponenty zadané soubory/cesty.
		/// @param component - jméno komponenty
		/// @param group - jméno skupiny
		/// @param entries - seznam souborů, složek a vzorů
		void exclude_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& exclude_list = find_file_list(component.exclude, group);

			for (cr<string> entry : entries)
			{
				fs::path shifted = shift(entry, component.location);
				if (is_outside_component(shifted))
				{
					report_error(ERROR::file_outside_component);
				}
				else
				{
					add_if_not_presented(exclude_list, shifted.generic_string());
				}
			}
		}
		/// @brief Odebere soubory ze seznamu začleněných/vyčleněných souborů.
		/// @param component - jméno komponenty
		/// @param group - jméno skupiny
		/// @param entries - seznam souborů, složek a vzorů
		void remove_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& include_list = find_file_list(component.include, group);
			vector<string>& exclude_list = find_file_list(component.exclude, group);

			for (cr<string> entry : entries)
			{
				fs::path shifted = shift(entry, component.location);
				if (is_outside_component(shifted))
				{
					report_error(ERROR::file_outside_component);
					continue;
				}

				int count = erase(exclude_list, shifted.generic_string());

				if (count == 0)
				{
					erase(include_list, shifted.generic_string());
				}
			}
		}

		/// @brief Stáhne a vypíše seznam komponent ze serveru.
		/// @param url - url serveru, ze kterého bude stažen seznam komponent
		static void list_server_components(cr<string> url)
		{
			client_ptr client = select_client(url, "");
			vector<string> components = client->list_components();
			for (cr<string> component : components)
			{
				println(cout, "{}", component);
			}
		}
	};
}
