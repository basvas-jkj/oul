module;

#include <boost/filesystem.hpp>
#include <boost/url.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>

export module support;

using namespace std;
using namespace boost::urls;
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
	/// Zaručuje, že výstup nikdy nekončí na /.
	/// Pokud <code>url</code> nekončí na /, připojí ho.
	/// @param url - prodlužovaná URL
	/// @param next - nová část
	export string url_append(string url, cr<string> next)
	{
		if (next != "" && !url.ends_with('/'))
		{
			url.append("/");
		}

		url.append(next);

		if (url.ends_with('/'))
		{
			url.pop_back();
		}
		return url;
	}
	/// @brief Kontroluje, zda je řetězec platná URL s podporovaným schématem nebo absolutní cesta v
	/// souborovém systému.
	/// @param url - validovaný řetězec
	/// @return <code>true</code>, pokud argument představuje validní url, <code>false</code> v
	/// opačném případě
	export bool validate_url(cr<string> url)
	{
		set<string> supported_schemas = {"file", "http", "ftp", "scp"};

		auto&& parsed = parse_absolute_uri(url);
		if (parsed.has_value() && supported_schemas.contains(parsed.value().scheme()))
		{
			return true;
		}

		return path(url).is_absolute();
	}
}
