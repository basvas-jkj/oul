#pragma once

#include <string>
#include "support.hpp"
#include "config/config.hpp"

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
	 * @return Je-li načtená konfigurace platná, vrátí <code>true</code> a staženou komponentu. Jinak vrátí <code>false</code>.
	 */
	optional<ITEM> download(const std::string& url, const std::string& where);
}
