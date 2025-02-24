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
static bool is_pattern(auto&& name)
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
 * @brief Zkontroluje existenci zadaných souborů, jdeli o konkrétní cestu.
 * @param entries - seznam cest a vzorů
 * @return <code>true</code>, pokud všehny cesty odkazují na existující soubor, <code>false</code> v opačném případě
 **/
static bool check_files_existence(auto&& entries)
{
	for (cr<string> entry : entries)
	{
		bool is_valid = is_pattern(entry) || fs::exists(entry);

		println("{} {}", entry, is_valid);

		if (!is_valid)
		{
			return false;
		}
	}
	return true;
}

namespace oul
{
	/**
	 * @brief Konfigurace neobsahuje hledanou komponentu.
	 **/
	export class MissingComponent: public CommonException
	{
	public:
		MissingComponent(cr<string> m): CommonException(m)
		{}
		MissingComponent(string&& m): CommonException(move(m))
		{}
	};
	/**
	 * @brief Konfigurace komponenty neobsahuje hledanou skupinu.
	 **/
	export class MissingGroup: public CommonException
	{
	public:
		MissingGroup(cr<string> m): CommonException(m)
		{}
		MissingGroup(string&& m): CommonException(move(m))
		{}
	};

	/**
	 * @brief Třída pro vytváření a správu komponent.
	 **/
	export class COMPONENT_MANAGER
	{
		CONFIG cfg;

		ITEM& find_component(cr<string> component)
		{
			vector<ITEM>::iterator component_it = cfg.get_component(component);
			if (component_it == cfg.components.end())
			{
				throw MissingComponent(message::component_not_found);
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
				throw MissingGroup(message::group_not_found);
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

			if (check_files_existence(entries))
			{
				include_list.append_range(entries);
			}
			else
			{
				report_error(message::file_not_exist);
				return;
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
			exclude_list.append_range(entries);
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
				int count = erase(exclude_list, entry);

				if (count == 0)
				{
					erase(include_list, entry);
				}
			}
		}
	};
}
