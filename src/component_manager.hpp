#pragma once

#include <string>

#include "config.hpp"

namespace oul
{
	/**
	 * @brief Přidá do konfigurace zadané komponenty nové soubory (nebudou vytvořeny ani přepsány).
	 * @param c konfigurace projektu
	 * @param name místní jméno komponenty
	 * @param source_files seznam přidávaných souborů zdrojového kódu
	 * @param test_files seznam přidávaných souborů testů
	 * @param doc_files seznam přidávaných souborů dokumentace
	 * @param where umístění komponenty
	 **/
	void add_files(CONFIG& c, const std::string& name, const std::string& source_files, const std::string& test_files, const std::string& doc_files);
	/**
	 * @brief Stáhne novou komponentu ze serveru a přidá ji do projektu.
	 * @param c konfigurace projektu
	 * @param name jméno komponenty na serveru
	 * @param save_as jméno, pod kterým bude komponenta zaznamenaná v konfiguraci
	 * @param url adresa serveru, ze které bude komponenta stažena
	 **/
	void add_component(CONFIG& c, const std::string& name, const std::string& save_as, const std::string& url, const std::string& where);
	/**
	 * @brief Vytvoří novou komponentu v projektu.
	 * @param c konfigurace projektu
	 * @param name jméno komponenty
	 * @param where umístění komponenty
	 * @param source_files seznam souborů zdrojového kódu, které budou patřit do vytvořené komponenty
	 * @param test_files seznam souborů testů, které budou patřit do vytvořené komponenty
	 * @param doc_files seznam souborů dokumentace, které budou patřit do vytvořené komponenty
	 **/
	void create_component(CONFIG& c, const std::string& name, const std::string& where, const std::string& source_files, const std::string& test_files, const std::string& doc_files);
	/**
	 * @brief Přejmenuje komponentu v konfiguraci projektu.
	 * @param c konfigurace projektu
	 * @param old_name původní jméno komponenty
	 * @param new_name nové jméno komponenty
	 **/
	void rename_component(CONFIG& c, const std::string& old_name, const std::string& new_name);
}