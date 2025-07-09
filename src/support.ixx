module;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <map>
#include <string>
#include <vector>

export module support;

using namespace std;
using namespace boost::filesystem;

namespace oul
{
	/// @brief Identifier of the current oul version.
	export const string version_id = "1.1";

	/// @brief Zkratka pro konstantní referenci.
	/// @tparam T - Libovolný typ, u kterého dává smysl mít konstantní reference.
	export template <class T>
	using cr = const T&;

	/// @brief Seznam souborů členěný podle kategorií.
	export using file_map = map<string, vector<string>>;

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
	/// @brief Připojí řetězec <code>next</code> na konec <code>url</code>.
	/// Pokud <code>url</code> nekončí na /, připojí ho.
	/// @param url - prodlužovaná URL
	/// @param next - nová část
	export string url_append(string url, cr<string> next)
	{
		if (next == "")
		{
			return url;
		}
		else if (!url.ends_with('/'))
		{
			url.append("/");
		}

		url.append(next);
		return url;
	}
}
