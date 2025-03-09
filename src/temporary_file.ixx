module;

#include <boost/filesystem.hpp>

export module tmp;

import std;
import usings;

using namespace std;
using namespace boost::filesystem;

namespace oul
{
	/**
	 * @brief Vytvoří složku pro dočasné soubory (podsložka oul v systémové složce).
	 * @param use_subfolder Vytvoří se také podsložka zajišťující unikátnost mezi různými voláními programu oul?
	 * @return umístění složky pro dočasné soubory
	 **/
	path get_temporary_folder(bool use_subfolder)
	{
		path temp = temp_directory_path() / "oul";

		if (use_subfolder)
		{
			temp /= unique_path();
		}

		create_directory(temp);
		return temp;
	}

	/**
	 * @brief Třída reprezentující dočasný soubor.
	 **/
	class TMP_FILE
	{
		/**
		 * @brief Jméno dočasného souboru.
		 **/
		path name;
	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FILE</code>.
		 * @param name jméno dočasného souboru (ekvivalent argumentu funkce <code>boost::filesystem::unique_path</code>)
		 **/
		TMP_FILE(cr<string> name, bool use_subfolder): name(unique_path(get_temporary_folder(use_subfolder) / name))
		{}
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FILE</code>.
		 * @param name přesné umístění souboru
		 **/
		TMP_FILE(cr<path> name): name(name) {}
		/**
		 * @brief Odstraní dočasný soubor ze souborového systému.
		 **/
		~TMP_FILE() noexcept
		{
			try
		{
			remove(name);
		}
			catch (...)
			{
				println("Unexpected destructror failure.");
			}
		}

		/**
		 * @return jméno dočasného souboru
		 **/
		path get_name() const
		{
			return name;
		}
		/**
		 * @brief Otevře dočasný soubor pro čtení.
		 * @return Objekt typu <code>ifstream</code> reprezentující dočasný soubor.
		 **/
		ifstream read() const
		{
			return ifstream(name.c_str());
		}
		/**
		 * @brief Otevře dočasný soubor pro zápis.
		 * @return Objekt typu <code>ofstream</code> reprezentující dočasný soubor.
		 **/
		ofstream write() const
		{
			return ofstream(name.c_str());
		}
	};

	/**
	 * @brief Třída reprezentující dočasnou složku.
	 **/
	class TMP_FOLDER
	{
		path name;
	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FOLDER</code>.
		 * @param name jméno dočasné složky (ekvivalent argumentu funkce <code>boost::filesystem::unique_path</code>)
		 **/
		TMP_FOLDER(cr<string> name, bool use_subfolder): name(unique_path(get_temporary_folder(use_subfolder) / name))
		{
			create_directory(name);
		}
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>TMP_FOLDER</code>.
		 * @param name přesné umístění dočasné složky
		 **/
		TMP_FOLDER(cr<path> name): name(name)
		{
			create_directory(name);
		}
		/**
		 * @brief Odstraní dočasnou složku ze souborového systému.
		 **/
		~TMP_FOLDER() noexcept
		{
			try
		{
			remove_all(name);
		}
			catch (...)
			{
				println("Unexpected destructror failure.");
			}
		}

		/**
		 * @return jméno dočasné složky
		 **/
		path get_name() const
		{
			return name;
		}
	};
}
