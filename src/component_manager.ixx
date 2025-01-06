export module component_manager;

import <string>;
import <optional>;
import config;
import usings;

using namespace std;

export namespace oul
{
	/**
	 * @brief Třída pro vytváření a správu komponent.
	 **/
	class COMPONENT_MANAGER
	{
		CONFIG c;

	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>COMPONENT_MANAGER</code>.
		 **/
		COMPONENT_MANAGER(CONFIG&& c): c(move(c))
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
			c.add_component(i);
		}
	};
}
