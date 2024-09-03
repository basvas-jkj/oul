#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace oul
{
    /**
     * @brief Typ představující nepovinnou hodnotu libovolného datového typu.
     * @tparam T datový typ, jehož hodnota je nepovinná
     **/
    template <class T>
    using optional = std::pair<bool, T>;

    /**
     * @brief Přesune soubor. Pokud cílová cesta neexistuje, bude vytvořena.
     * @param source_directory původní umístění
     * @param target_directory nové umístění
     * @param entry_name jméno souboru či složky
     **/
    void move(const fs::path& source_directory, const std::string& target_directory, const std::string& entry_name);
}
