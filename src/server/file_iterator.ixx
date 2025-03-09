module;

#include <boost/filesystem.hpp>

#define __clang__
#include <wildcards/wildcards.hpp>
#undef __clang__

export module dir_iterator;

import std;
import usings;

using namespace std;
using namespace wildcards;
using namespace boost::filesystem;
namespace fs = boost::filesystem;
using recursive_it = boost::filesystem::recursive_directory_iterator;

namespace oul
{
	/**
	 * @return Shoduje se položka s alespoň jedním vzorem?
	 **/
	bool match_any(cr<string> entry, cr<set<string>> patterns)
	{
		for (cr<string> pattern : patterns)
		{
			if (match(entry, pattern).res)
			{
				return true;
			}
		}
		return false;
	}
	/**
	 * @return Shoduje se položka s alespoň jedním vzorem?
	 **/
	bool match_any(cr<path> base, cr<directory_entry> entry, cr<set<string>> patterns)
	{
		cr<string> entry_string = relative(entry.path(), base).generic_string();
		return match_any(entry_string, patterns);
	}
	/**
	 * @brief Spojí seznam souborů do jedné množiny.
	 * @param list - Seznam souborů strukturovaný podle kategorií.
	 **/
	set<string> join_filemap(cr<file_map> list)
	{
		set<string> files;
		for (auto&& group : list)
		{
			files.insert_range(group.second);
		}
		return files;
	}

	/**
	 * @brief Rekurzivní iterátor souborového systému, který umožňuje specifikovat, které soubory
	 * budou nebo nebudou zahrnuty. Vrací pouze soubory.
	 **/
	export class FILE_ITERATOR
	{
		path base;
		recursive_it it;
		recursive_it end_it;

		set<string> include;
		set<string> exclude;

		/**
		 * @brief Vytvoří <code>FILE_ITERATOR</code> z rekurzivního iterátoru.
		 */
		FILE_ITERATOR(cr<recursive_it> it): it(it), end_it(it)
		{}
		/**
		 * @brief Vytvoří <code>FILE_ITERATOR</code> pro zadanou cestu.
		 * @param base - kořenová složky iterátoru
		 * @param inc - množina zahrnutých souborů/složek
		 * @param exc - množina nezahrnutých souborů/složek
		 **/
		FILE_ITERATOR(cr<path> base, set<string>&& inc, set<string>&& exc): 
			base(base), it(base), end_it(fs::end(it)), include(move(inc)), exclude(move(exc))
		{}

	public:
		/**
		 * @param base - Kořen iterátoru.
		 * @param include - soubory/složky/vzory cest, které iterátor vrací (předané jako file_map)
		 * @param exclude - soubory/složky/vzory cest, které iterátor nevrací (předané jako file_map) 
		 * @return připravený iterátor
		 **/
		static FILE_ITERATOR init(cr<path> base, cr<file_map> include, cr<file_map> exclude)
		{
			set<string> joined_include = join_filemap(include);
			set<string> joined_exclude = join_filemap(exclude);
			FILE_ITERATOR dir_it(base, move(joined_include), move(joined_exclude));

			if (dir_it.it->is_directory() || match_any(base, *dir_it.it, dir_it.exclude) || !match_any(base, *dir_it.it, dir_it.include))
			{
				++dir_it;
			}

			return dir_it;
		}

	private:
		bool include_all = false;
		int base_depth;
	public:
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

				if (match_any(base, *it, exclude))
				{
					it.disable_recursion_pending();
				}
				else if (include_all)
				{
					if (base_depth >= it.depth())
					{
						include_all = false;
						continue;
					}
					else if (it->is_regular_file())
					{
						break;
					}
				}
				else if (match_any(base, *it, include))
				{
					if (it->is_directory())
					{
						include_all = true;
						base_depth = it.depth();
					}
					else
					{
						break;
					}
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
	};
}
