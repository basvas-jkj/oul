export module component_manager;

import <string>;
import <optional>;
import config;

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
		COMPONENT_MANAGER(CONFIG&& c): c(c)
		{};
		/**
		 * @brief Vytvoří novou komponentu v projektu.
		 * @param c konfigurace projektu
		 * @param name jméno komponenty
		 * @param where umístění komponenty
		 **/
		void create(cr<string> name, cr<string> where)
		{
			if (c.contains(name))
			{
				cerr << "This project already contains a component of the same name." << endl;
				return;
			}

			ITEM i(name, location);
			c.add_component(i);
		}
	};
}
