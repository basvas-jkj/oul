export module component_manager;

import <boost/filesystem.hpp>;
import std;
import config;
import usings;
import message;

using namespace std;
namespace fs = boost::filesystem;

/**
 * @brief Zkontroluje, zda je řetězec vzor nebo konkrétní cesta.
 * @param name kontrolovaný řetězec
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
 * @param entries seznam cest a vzorů
 * @return <code>true</code>, pokud všehny cesty odkazují na existující soubor, <code>false</code> v opačném případě
 **/
static bool check_files_existence(auto&& entries)
{
	for (cr<string> entry : entries)
	{
		if (!fs::exists(entry) && !is_pattern(entry))
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
	export class MissingComponent: public exception
	{
		string message;

	public:
		MissingComponent(cr<string> m): message(m)
		{}
		MissingComponent(string&& m): message(m)
		{}
		void report() const
		{
			report_error(message);
		}
	};
	/**
	 * @brief Konfigurace komponenty neobsahuje hledanou skupinu.
	 **/
	export class MissingGroup: public exception
	{
		string message;

	public:
		MissingGroup(cr<string> m): message(m)
		{}
		MissingGroup(string&& m): message(m)
		{}
		void report() const
		{
			report_error(message);
		}
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
				throw MissingComponent(message::group_not_found);
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
		 * @param c konfigurace projektu
		 * @param name jméno komponenty
		 * @param location umístění komponenty
		 **/
		void create(cr<string> name, cr<string> location)
		{
			ITEM i(name, location);
			cfg.add_component(i);
		}
		/**
		 * @brief Přidá do konfigurace zadané komponenty nové soubory (musí existovat) nebo vzory cest.
		 * @param component jméno komponenty
		 * @param group jméno skupiny
		 * @param entries seznam souborů, složek a vzorů
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
		 * @param component jméno komponenty
		 * @param group jméno skupiny
		 * @param entries seznam souborů, složek a vzorů
		 **/
		void exclude_files(cr<string> name, cr<string> group, span<const string> entries)
		{
			ITEM& component = find_component(name);
			vector<string>& exclude_list = find_file_list(component.exclude, group);
			exclude_list.append_range(entries);
		}
		/**
		 * @brief Odebere soubory ze seznamu začleněných/vyčleněných souborů.
		 * @param component jméno komponenty
		 * @param group jméno skupiny
		 * @param entries seznam souborů, složek a vzorů
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
