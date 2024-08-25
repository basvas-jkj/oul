#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace oul
{
    /**
     * @brief Přesune soubor. Pokud cílová cesta neexistuje, bude vytvořena.
     * @param source_directory původní umístění
     * @param target_directory nové umístění
     * @param entry_name jméno souboru či složky
     **/
    void move(const fs::path& source_directory, const std::string& target_directory, const std::string& entry_name);
    /**
     * @brief Vytvoří složku pro dočasné soubory (podsložka oul v systémové složce).
     * @param use_subfolder Vytvoří se podsložka zajišťující unikátnost mezi různými voláními programu oul?
     * @return umístění složky pro dočasné soubory
     **/
    fs::path get_temporary_folder(bool use_subfolder = false);
}
