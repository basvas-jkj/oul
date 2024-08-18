#pragma once

#include <string>
#include "config.hpp"

namespace oul
{
	/**
	 * @brief Zaregistruje nového uživatele na serveru.
	 * @param c Konfigurace projektu.
	 **/
	void registration(const CONFIG&);
	/**
	 * @brief Stáhne zazipovanou komponentu ze serveru.
	 * @param url Adresa, ze které bude komponenta stažena
	 * @return Cesta ke staženému archivu. Pokud stažení nebylo úspìšné, vrátí "".
	 */
	std::string download(const std::string&);
}
