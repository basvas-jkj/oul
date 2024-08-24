#pragma once

#include <ranges>
#include <string>
#include <vector>
#include <algorithm>

namespace oul
{
    template <class T>
    using optional = std::pair<bool, T>;

    struct ITEM
    {
        std::string name;
        std::string location;
        std::string original_name;
        std::string url;
        std::vector<std::string> documentation;
        std::vector<std::string> source_files;
        std::vector<std::string> test_files;
    };
    
    /**
     * @brief Třída pro ukládání konfigurace projektu.
     **/
    struct CONFIG
    {
        std::string location;
        std::string name;
        std::string default_url;
        std::vector<ITEM> components;

        /**
         * @brief Konstruktor vytvářející prázdný objekt structury <code>CONFIG</code>.
         **/
        CONFIG() {}
        /**
         * @brief Konstruktor vytvářející objekt structury <code>CONFIG</code>.
         * @param l umístění konfigurace v souborovém systému
         **/
        CONFIG(const std::string& l): location(l)
        {}
        /**
         * @brief Zajišťuje zápsání změněné konfigurace do souboru.
         **/
        ~CONFIG();

        /**
         * @brief Najde cestu ke konfiguračnímu souboru.Pokud je soubor nalezen, nastaví pracovní složku na jeho umístění.
         * @return Pokud byla konfigurace nalezena, vrátí cestu. Jinak vrátí "".
         **/
        static std::string find();
        /**
         * @brief Najde konfigurační soubor a načte ho.
         * @return Je-li načtená konfigurace platná, vrátí <code>true</code> a načtenou komponentu. Jinak vrátí <code>false</code>.
         **/
        static optional<CONFIG> read();
        /**
         * @brief Načte konfiguraci ze zadaného souboru.
         * @param config_file umístění konfiguračního souboru souboru
         * @return Je-li načtená konfigurace platná, vrátí <code>true</code> a načtenou komponentu. Jinak vrátí <code>false</code>.
         **/
        static optional<CONFIG> read(const std::string&);
        /**
         * @brief Vytvoří prázdnou konfiguraci. Potřebné údaje získá interaktivně ze standardního vstupu.
         **/
        static void initialize();

        /**
         * @brief Přidá komponentu do konfigurace. 
         * @param i konfigurace komponenty
         **/
        void add_component(const ITEM& i);
        /**
         * @brief Vypíše seznam všech komponent v konfiguraci.
         **/
        void list_components();
        /**
         * @brief Převede seznam všech komponent v konfiguraci na seznam názvů.
         * @return iterovatelný seznam (<code>view</code>) názvů všech komponent
         **/
        auto get_components()
        {
            return components | std::views::transform([](const ITEM& i){return i.name;});
        }
        /**
         * @brief Zjistí, zda konfigurace projektu obsahuje komponentu daného jména.
         * @param name jméno komponenty
         * @return Je v konfiguraci přítomná komponenta?
         **/
        bool contains(const std::string& name);
        /**
         * @brief Najde v konfiguraci projektu komponentu a vrátí iterátor.
         * @param name jméno komponenty
         * @return Platný iterátor ukazující na komponentu, pokud je v konfiguraci přítomná. Jinak vrátí <code>end()</code>
         **/
        std::vector<ITEM>::iterator get_component(const std::string& name);

        /**
         * @brief Získá výchozí URL pro stažení komponenty.
         * @param  jméno komponenty
         * @return spojení výchozí URL projektu a jména komponenty
         **/
        std::string get_url(const std::string&) const;
    };
}