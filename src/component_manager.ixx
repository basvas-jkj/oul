export module component_manager;

import <string>;
import <optional>;
import config;
import usings;
import message;

using namespace std;

export namespace oul
{
	/**
	 * @brief Třída pro vytváření a správu komponent.
	 **/
	class COMPONENT_MANAGER
	{
		CONFIG cfg;

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
		 * @brief Přidá do konfigurace zadané komponenty nové soubory (nebudou vytvořeny ani přepsány).
		 * @param c konfigurace projektu
		 * @param name místní jméno komponenty
		 * @param source_files seznam přidávaných souborů zdrojového kódu
		 * @param test_files seznam přidávaných souborů testů
		 * @param doc_files seznam přidávaných souborů dokumentace
		 * @param where umístění komponenty
		 **/
		void add_files(cr<string> component, cr<string> group, span<const string> files)
		{
			vector<ITEM>::iterator i = cfg.get_component(component);
			if (i == cfg.components.end())
			{
				report_error(message::component_not_found);
				return;
			}
			
			file_map::iterator g = i->include.find(group);
			if (g == i->include.end())
			{
				report_error(message::group_not_found);
				return;
			}
			
			g->second.append_range(files);
		}
		void exclude_files(cr<string> component, cr<string> group, span<const string> files)
		{
			vector<ITEM>::iterator i = cfg.get_component(component);
			if (i == cfg.components.end())
			{
				report_error(message::component_not_found);
				return;
			}
			
			file_map::iterator g = i->include.find(group);
			if (g == i->include.end())
			{
				report_error(message::group_not_found);
				return;
			}
			i->exclude["group"].append_range(files);
		}
	};
}
