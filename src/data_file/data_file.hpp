#pragma once

#include <tuple>
#include <string>

#include "../config.hpp"

namespace oul
{
    /**
     * @brief Typ představující nepovinnou hodnotu libovolného datového typu.
     * @tparam T datový typ, jehož hodnota je nepovinná
     **/
    template <class T>
    using optional = std::pair<bool, T>;

    /**
     * @brief Přečte souboru a jeho obsah převede na komponentu.
     * @param s soubor, ze kterého se komponenta načte
     * @return Pokud soubor obsahuje komponentu, vrátí <code>true</code> a přečtenou komponentu. Jinak vrátí <code>false</code>.
     **/
    optional<ITEM> read_component(const std::string&);
    /**
     * @brief Načte konfiguraci ze souboru.
     * @param config_file Soubor, ze kterého se načte konfigurace.
     * @return Je-li konfigurace platná, vrátí <code>true</code> a načtenou konfiguraci. Jinak vrátí <code>false</code>.
     **/
    optional<CONFIG> load_config(const std::string&);

    /**
     * @brief Zapíše konfiguraci komponenty do souboru podle zvoleného formátu.
     * @param i objekt nesoucí konfiguraci komponenty
     * @param format výstupní formát souboru
     **/
    void write_component(const ITEM& i, const std::string& format);
    /**
     * @brief Zapíše konfiguraci projektu do souboru podle zvoleného formátu.
     * @param cfg objekt nesoucí zapisovanou konfiguraci
     **/
    void write_config(const CONFIG&);
}

