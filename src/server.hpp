#pragma once

#include <string>
#include "config.hpp"

namespace oul
{
	/**
	 * @brief Zaregistruje nov�ho u�ivatele na serveru.
	 * @param c Konfigurace projektu.
	 **/
	void registration(const CONFIG&);
	/**
	 * @brief St�hne zazipovanou komponentu ze serveru.
	 * @param url Adresa, ze kter� bude komponenta sta�ena
	 * @return Cesta ke sta�en�mu archivu. Pokud sta�en� nebylo �sp�n�, vr�t� "".
	 */
	std::string download(const std::string&);
}
