#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>

#include "config.hpp"

namespace oul
{
	/**
	 * @brief Třída usnadňující práci se zazipovanými komponentami.
	 **/
	class ZIP_MANAGER
	{
		ITEM c;
		std::string zip_file;
		boost::filesystem::path extracted_directory;

	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>ZIP_MANAGER</code>. Ze zazipované komponenty načte její konfiguraci.
		 * @param zip Umístění zazipované komponenty.
		 **/
		ZIP_MANAGER(const std::string& zip);
		/**
		 * @brief Odstraní zazipovanou komponentu a všechny dočasné soubory.
		 **/
		~ZIP_MANAGER();

		/**
		 * @brief Rozbalí obsah komponenty.
		 * @param where Složka, do které je komponenta rozbalena.
		 * @return konfigurace rozbalené komponenty
		 **/
		ITEM unzip(const std::string& where);
	};
}
