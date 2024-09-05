#pragma once

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace oul
{
	/**
	 * @brief Vytvoří složku pro dočasné soubory (podsložka oul v systémové složce).
	 * @param use_subfolder Vytvoří se také podsložka zajišťující unikátnost mezi různými voláními programu oul?
	 * @return umístění složky pro dočasné soubory
	 **/
	fs::path get_temporary_folder(bool use_subfolder);

	/**
	 * @brief Třída reprezentující dočasný soubor.
	 **/
	class TMP_FILE
	{
		fs::path name;
	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FILE</code>.
		 * @param name jméno dočasného souboru (ekvivalent argumentu funkce <code>boost::filesystem::unique_path</code>)
		 **/
		TMP_FILE(const std::string& name, bool use_subfolder);
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FILE</code>.
		 * @param name přesné umístění souboru
		 **/
		TMP_FILE(const fs::path& name);
		/**
		 * @brief Odstraní dočasný soubor ze souborového systému.
		 **/
		~TMP_FILE();

		/**
		 * @return jméno dočasného souboru
		 **/
		fs::path get_name() const;
		/**
		 * @brief Otevře dočasný soubor pro čtení.
		 * @return Objekt typu <code>ifstream</code> reprezentující dočasný soubor.
		 **/
		std::ifstream read() const;
		/**
		 * @brief Otevře dočasný soubor pro zápis.
		 * @return Objekt typu <code>ofstream</code> reprezentující dočasný soubor.
		 **/
		std::ofstream write() const;
	};

	/**
	 * @brief Třída reprezentující dočasnou složku.
	 **/
	class TMP_FOLDER
	{
		fs::path name;
	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FOLDER</code>.
		 * @param name jméno dočasné složky (ekvivalent argumentu funkce <code>boost::filesystem::unique_path</code>)
		 **/
		TMP_FOLDER(const std::string& name, bool use_subfolder);
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FOLDER</code>.
		 * @param name přesné umístění dočasné složky
		 **/
		TMP_FOLDER(const fs::path& name);
		/**
		 * @brief Odstraní dočasnou složku ze souborového systému.
		 **/
		~TMP_FOLDER();

		/**
		 * @return jméno dočasné složky
		 **/
		fs::path get_name() const;
	};
}
