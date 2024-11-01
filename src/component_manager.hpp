#pragma once

#include <string>

#include "config/config.hpp"

namespace oul
{
	/**
	 * @brief Třída pro vytváření a správu komponent.
	 **/
	class COMPONENT_MANAGER
	{
		CONFIG c;

		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>COMPONENT_MANAGER</code>.
		 **/
		COMPONENT_MANAGER(CONFIG&& c);
	public:
		/**
		 * @brief Najde konfiguraci projektu a připraví objekt typu <code>COMPONENT_MANAGER</code>.
		 * @return Pokud byla konfigurace nalezena, vrátí <code>true</code> a objekt typu <code>COMPONENT_MANAGER</code>. Jinak vrátí <code>false</code>.
		 **/
		static optional<COMPONENT_MANAGER> init();
		/**
		 * @brief Přidá do konfigurace zadané komponenty nové soubory (nebudou vytvořeny ani přepsány).
		 * @param c konfigurace projektu
		 * @param name místní jméno komponenty
		 * @param source_files seznam přidávaných souborů zdrojového kódu
		 * @param test_files seznam přidávaných souborů testů
		 * @param doc_files seznam přidávaných souborů dokumentace
		 * @param where umístění komponenty
		 **/
		void add_files(const std::string& name, const std::string& source_files, const std::string& test_files, const std::string& doc_files);
		/**
		 * @brief Odebere z konfigurace zadané soubory dané komponenty (neodstraní je).
		 * @param c konfigurace projektu
		 * @param name místní jméno komponenty
		 * @param source_files seznam odebíraných souborů zdrojového kódu
		 * @param test_files seznam odebíraných souborů testů
		 * @param doc_files seznam odebíraných souborů dokumentace
		 **/
		void remove_files(const std::string& name, const std::string& source_files, const std::string& test_files, const std::string& doc_files);
		/**
		 * @brief Stáhne novou komponentu ze serveru a přidá ji do projektu.
		 * @param c konfigurace projektu
		 * @param name jméno komponenty na serveru
		 * @param save_as jméno, pod kterým bude komponenta zaznamenaná v konfiguraci
		 * @param url adresa serveru, ze které bude komponenta stažena
		 **/
		void install(const std::string& name, const std::string& save_as, const std::string& url, const std::string& where);
		/**
		 * @brief Odtraní zadanou komponentu a všechny její soubory.
		 * @param c c konfigurace projektu
		 * @param name místní jméno komponenty
		 **/
		void remove(const std::string& name);
		/**
		 * @brief Vytvoří novou komponentu v projektu.
		 * @param c konfigurace projektu
		 * @param name jméno komponenty
		 * @param where umístění komponenty
		 * @param source_files seznam souborů zdrojového kódu, které budou patřit do vytvořené komponenty
		 * @param test_files seznam souborů testů, které budou patřit do vytvořené komponenty
		 * @param doc_files seznam souborů dokumentace, které budou patřit do vytvořené komponenty
		 **/
		void create(const std::string& name, const std::string& where, const std::string& source_files, const std::string& test_files, const std::string& doc_files);
		/**
		 * @brief Přejmenuje komponentu v konfiguraci projektu.
		 * @param c konfigurace projektu
		 * @param old_name původní jméno komponenty
		 * @param new_name nové jméno komponenty
		 **/
		void rename(const std::string& old_name, const std::string& new_name);
		/**
		 * @brief Změní umístění komponenty.
		 * @param c konfigurace projektu
		 * @param name místní jméno komponenty
		 * @param new_location nové umístění komponenty
		 **/
		void move(const std::string& name, const std::string& new_location);
	};
}