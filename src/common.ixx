module;

#include <boost/filesystem.hpp>

#define __clang__
#include <wildcards/wildcards.hpp>
#undef __clang__

export module common;

export import usings;
export import message;

using namespace std;
using namespace boost::filesystem;

namespace oul
{
    /// @brief Zkontroluje, zda položka odpovídá zadanému vzoru.
    /// @param entry - kontrolovaná položka
    /// @param pattern - vzor
    /// @return <code>true</code>, pokud položka vzoru odpovídá, <code>false</code> v opačném případě.
    export bool match(cr<string> entry, cr<string> pattern)
    {
        return wildcards::match(entry, pattern).res;
    }
    /// @brief Přesune soubor. Pokud cílová cesta neexistuje, bude vytvořena.
    /// @param source_directory - původní umístění
    /// @param target_directory - nové umístění
    /// @param entry_name - jméno souboru či složky
    export void move_file(cr<path> source_directory, cr<path> target_directory, cr<path> entry_name)
    {
        path source = source_directory / entry_name;
        path target = target_directory / entry_name;
        if (target.has_parent_path())
        {
            create_directories(target.parent_path());
        }

        rename(source, target);
    }
    /// @brief Zkopíruje soubor. Pokud cílová cesta neexistuje, bude vytvořena.
    /// @param source_directory - původní umístění
    /// @param target_directory - nové umístění
    /// @param entry_name - jméno souboru či složky
    export void copy_file(cr<path> source_directory, cr<path> target_directory, cr<path> entry_name)
    {
        path source = source_directory / entry_name;
        path target = target_directory / entry_name;
        if (target.has_parent_path())
        {
            create_directories(target.parent_path());
        }

        copy_file(source, target, copy_options::update_existing);
    }
    /// @brief Rozdělí řetězec na podřetězce podle zadaného oddělovače.
    /// @param s - rozdělovaný řetězec
    /// @param ch - oddělovač
    /// @return seznam podřetězců
    vector<string> split(cr<string> s, char ch)
    {
        vector<string> v;
        string::const_iterator start = s.begin();

        while (start != s.end())
        {
            string::const_iterator end = find(start, s.end(), ch);
            v.push_back(string(start, end));

            if (end == s.end())
            {
                break;
            }
            else
            {
                start = end + 1;
            }
        }

        return v;
    }

    /// @brief Základní třída pro výjimky použité v tomto projektu.
    export class CommonException: exception
    {
        ERROR name;

    public:
        CommonException(ERROR n): name(n)
        {}
        void report() const
        {
            report_error(name);
        }
    };
}
