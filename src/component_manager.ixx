module;

#include <boost/filesystem.hpp>

export module component_manager;

import std;
import usings;
import common;
import message;
export import config;

using namespace std;
namespace fs = boost::filesystem;

/**
 * @brief Zkontroluje, zda je řetězec vzor nebo konkrétní cesta.
 * @param name - kontrolovaný řetězec
 * @return <code>true</code>, pokud je řetězec vzor cesty, <code>false</code>, pokud jde o konkréní cestu
 **/
bool is_pattern(cr<string> name)
{
	vector<char> special_chars{'*', '?', '\\', '[', ']', '(', ')', '|' , '!'};
	for (char ch : special_chars)
	{
		if (name.contains(ch))
		{
			return true;
		}
	}
	return false;
}
/**
 * @brief Zkontroluje, zda je zadaná položka platná.
 * @param entry - cesta nebo vzor
 * @return <code>true</code>, pokud cesta odkazuje na existující soubor nebo jde-li o vzor, <code>false</code> v opačném případě
 **/
bool check_entry_validity(cr<string> entry)
{
	return is_pattern(entry) || fs::exists(entry);
}

namespace oul
{
	/**
	 * @brief Konfigurace neobsahuje hledanou komponentu.
	 **/
	export class MissingComponent: public CommonException
	{
	public:
		MissingComponent(ERROR name): CommonException(name)
		{}
	};
	/**
	 * @brief Konfigurace komponenty neobsahuje hledanou skupinu.
	 **/
	export class MissingGroup: public CommonException
	{
	public:
		MissingGroup(ERROR name): CommonException(name)
		{}
	};

	/**
	 * @brief Třída pro vytváření a správu komponent.
	 **/
	export class COMPONENT_MANAGER
	{
		CONFIG cfg;

		fs::path shift(cr<fs::path> file) const
		{
			fs::path cfg_path = fs::path(cfg.location).parent_path();
			return fs::relative(file, cfg_path);
		}
		ITEM& find_component(cr<string> component)
		{
			vector<ITEM>::iterator component_it = cfg.get_component(component);
			if (component_it == cfg.components.end())
			{
				throw MissingComponent(component_not_found);
			}
			else
			{
				return *component_it;
			}
		}
		vector<string>& find_file_list(file_map& files, cr<string> group)
		{
			file_map::iterator group_it = files.find(group);
			if (group_it == files.end())
			{
				throw MissingGroup(group_not_found);
			}
			else
			{
				return group_it->second;
			}
		}

	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>COMPONENT_MANAGER</code>.
		 **/
		COMPONENT_MANAGER(CONFIG&& c): cfg(move(c))
		{};

		/**
		 * @brief Vytvoří novou komponentu v projektu.
		 * @param name - jméno komponenty
		 * @param location - umístění komponenty
		 **/
		void create(cr<string> name, cr<string> location)
		{
			ITEM i(name, location);
			cfg.add_component(i);
		}
		/**
		 * @brief Odebere komponentu z projektu.
		 * @param name - jméno komponenty
		 **/
		void remove(cr<string> name)
		{
			cfg.remove_component(name);
		}

		/**
		 * @brief Přidá skuspinu souborů do zadané komponenty.
		 * @param name - jméno komponenty
		 * @param group - jméno skupiny
		 **/
		void add_group(cr<string> name, cr<string> group)
		{
			ITEM& component = find_component(name);
			component.include.insert({group, vector<string>()});
			component.exclude.insert({group, vector<string>()});
		}
		/**
		 * @brief 
		 * @param name - jméno komponenty
		 * @param group - jméno skupiny
		 **/
		void remove_group(cr<string> name, cr<string> group)
		{
			ITEM& component = find_component(name);
			component.include.erase(group);
			component.exclude.erase(group);
		}

		/**
		 * @brief Přidá do konfigurace zadané komponenty nové soubory (musí existovat) nebo vzory cest.
		 * @param component - jméno komponenty
		 * @param group - jméno skupiny
		 * @param entries - seznam souborů, složek a vzorů
		 **/
		void add_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& include_list = find_file_list(component.include, group);

			for (cr<string> entry : entries)
			{
				if (check_entry_validity(entry))
				{
					include_list.push_back(shift(entry).string());
				}
				else
				{
					report_error(file_not_exist);
				}
			}
		}
		/**
		 * @brief Vyčlení z komponenty zadané soubory/cesty.
		 * @param component - jméno komponenty
		 * @param group - jméno skupiny
		 * @param entries - seznam souborů, složek a vzorů
		 **/
		void exclude_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& exclude_list = find_file_list(component.exclude, group);

			for (cr<string> entry : entries)
			{
				exclude_list.push_back(shift(entry).string());
			}
		}
		/**
		 * @brief Odebere soubory ze seznamu začleněných/vyčleněných souborů.
		 * @param component - jméno komponenty
		 * @param group - jméno skupiny
		 * @param entries - seznam souborů, složek a vzorů
		 **/
		void remove_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& include_list = find_file_list(component.include, group);
			vector<string>& exclude_list = find_file_list(component.exclude, group);

			for (cr<string> entry : entries)
			{
				string shifted = shift(entry).string();
				int count = erase(exclude_list, shifted);

				if (count == 0)
				{
					erase(include_list, shifted);
				}
			}
		}
	};
}
