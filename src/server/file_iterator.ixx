module;

#include <set>
#include <ranges>
#include <iterator>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>

export module file_iterator;

import common;

using namespace std;
using namespace boost::filesystem;

namespace oul
{
    export template <class T>
        concept DirectoryEntry = same_as<T, directory_entry> || requires(T entry)
    {
        { entry.is_regular_file() }->same_as<bool>;
        { entry.is_directory() }->same_as<bool>;
        { entry.depth() }->same_as<int>;
        same_as<path, remove_cvref_t<decltype(entry.path())>>;
    };
    export template <class T>
        concept DirectoryIterator = same_as<T, recursive_directory_iterator> || requires(T it)
    {
        input_iterator<T>;
        DirectoryEntry<typename T::value_type>;
    };



    /// @return Shoduje se položka s alespoň jedním vzorem?
    bool match_any(cr<string> entry, cr<vector<string>> patterns)
    {
        for (cr<string> pattern : patterns)
        {
            if (match(entry, pattern))
            {
                return true;
            }
        }
        return false;
    }
    /// @return Shoduje se položka s alespoň jedním vzorem?
    template <class DR>
        requires DirectoryEntry<DR> || same_as<DR, directory_entry>
    bool match_any(cr<path> base, cr<DR> entry, cr<vector<string>> patterns)
    {
        cr<string> entry_string = relative(entry.path(), base).generic_string();
        return match_any(entry_string, patterns);
    }
    /// @brief Spojí seznam souborů do jedné množiny.
    /// @param list - Seznam souborů strukturovaný podle kategorií.
    vector<string> join_filemap(cr<file_map> list)
    {
        set<string> files;
        for (auto&& group : list)
        {
            files.insert_range(group.second);
        }
        return vector<string>(files.begin(), files.end());
    }


    /// @brief Rekurzivní iterátor souborového systému, který umožňuje specifikovat, které soubory
    /// budou nebo nebudou zahrnuty. Vrací pouze soubory.
    export template <DirectoryIterator IT>
        class FILE_ITERATOR
    {
        path base;
        IT it;
        IT end_it;

        file_map include;
        file_map exclude;

        /// @brief Vytvoří <code>FILE_ITERATOR</code> z rekurzivního iterátoru.
        FILE_ITERATOR<IT>(cr<IT> it): it(it), end_it(it)
        {}

        template <class DR>
            requires DirectoryEntry<DR> || same_as<DR, directory_entry>
        bool should_be_included(cr<DR> entry)
        {
            for (auto&& [group_name, group_files] : include)
            {
                file_map::iterator it = exclude.find(group_name);
                bool included = match_any(base, entry, group_files);
                bool excluded = it != exclude.end() && match_any(base, entry, it->second);

                if (included && !excluded)
                {
                    return true;
                }
            }
            return false;
        }
        template <class DR>
            requires DirectoryEntry<DR> || same_as<DR, directory_entry>
        bool should_be_excluded(cr<DR> entry)
        {
            vector<string>&& all_excluded_patterns = join_filemap(exclude);
            return match_any(base, entry, all_excluded_patterns);
        }
    public:
        FILE_ITERATOR<IT>(IT&& it, IT&& end_it, cr<file_map> include, cr<file_map> exclude):
            it(it), end_it(end_it), include(include), exclude(exclude)
        {
            if (it == end_it)
            {
                return;
            }
            else if (it->is_directory())
            {
                if (should_be_included(*it))
                {
                    include_all = true;
                    include_all_base = depth();
                }
                else if (should_be_excluded(*it))
                {
                    exclude_all = true;
                    exclude_all_base = depth();
                }

                ++ * this;
            }
            else if (!should_be_included(*it))
            {
                ++*this;
            }
        }
        /// @param base - Kořen iterátoru.
/// @param include - soubory/složky/vzory cest, které iterátor vrací (předané jako file_map)
/// @param exclude - soubory/složky/vzory cest, které iterátor nevrací (předané jako file_map) 
/// @return připravený iterátor
        FILE_ITERATOR<recursive_directory_iterator>(cr<path> base, cr<file_map> include, cr<file_map> exclude):
            FILE_ITERATOR<IT>(recursive_directory_iterator(base), recursive_directory_iterator(), include, exclude)
        {}

    private:
        bool include_all = false;
        bool exclude_all = false;
        int include_all_base;
        int exclude_all_base;

        int depth()
        {
            if constexpr (same_as<IT, recursive_directory_iterator>)
            {
                return it.depth();
            }
            else
            {
                return it->depth();
            }
        }
    public:
        // -------------------------------------
        //     funkce, operátory a aliasy,
        //     díky kterým lze FILE_ITERATOR
        //     považovat za platný iterátor
        // -------------------------------------

        FILE_ITERATOR& begin()
        {
            return *this;
        }
        FILE_ITERATOR end()
        {
            return FILE_ITERATOR(end_it);
        }
        FILE_ITERATOR& operator++()
        {
            ++it;

            while (it != end_it)
            {
                auto s = it->path().string();

                if (exclude_all)
                {
                    if (exclude_all_base >= depth())
                    {
                        exclude_all = false;
                        continue;
                    }
                }
                else if (include_all && !should_be_excluded(*it))
                {
                    if (include_all_base >= depth())
                    {
                        include_all = false;
                        continue;
                    }
                    else if (it->is_regular_file())
                    {
                        break;
                    }
                }
                else if (should_be_included(*it))
                {
                    if (it->is_directory())
                    {
                        include_all = true;
                        include_all_base = depth();
                    }
                    else
                    {
                        break;
                    }
                }
                else if (should_be_excluded(*it))
                {
                    exclude_all = true;
                    exclude_all_base = depth();
                }

                ++it;
            }

            return *this;
        }
        FILE_ITERATOR operator++(int)
        {
            FILE_ITERATOR fi = *this;
            ++*this;
            return fi;
        }
        bool operator==(cr<FILE_ITERATOR> o)
        {
            return it == o.it;
        }
        bool operator!=(cr<FILE_ITERATOR> o)
        {
            return !(*this == o);
        }
        path operator*()
        {
            return it->path().generic_path();
        }
        path operator->()
        {
            return it->path().generic_path();
        }

        using value_type = path;
        using difference_type = ptrdiff_t;
        using pointer = FILE_ITERATOR;
        using reference = path;
        using iterator_category = input_iterator_tag;
    };
    FILE_ITERATOR(cr<path>, cr<file_map>, cr<file_map>)->FILE_ITERATOR<recursive_directory_iterator>;
    template <DirectoryIterator IT>
    FILE_ITERATOR(IT&&, cr<file_map>, cr<file_map>) -> FILE_ITERATOR<IT>;
}
