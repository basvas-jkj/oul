module;

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#undef ERROR

export module zip:base;

import tmp;
import config;
import common;
import file_iterator;

using namespace std;
using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
	/// @brief Chyba: nenalezen žádný zipovací nástroj.
	export class MissingZipTool: public CommonException
	{
	public:
		MissingZipTool(ERROR name): CommonException(name)
		{}
	};
	/// @brief Chyba: vytváření nebo rozbalování archovu selhalo
	export class ZippingError: public CommonException
	{
	public:
		ZippingError(ERROR name): CommonException(name)
		{}
	};

	template <class ...T>
	concept CmdArgs = ((same_as<string, T> || same_as<path, T>) && ...);

	/// @brief Základní typ zipovacích nástrojů.
	export class ABSTRACT_ZIP_TOOL
	{
		/// @brief Vytvoří zip archiv pro danou komponentu, zapíše do ní konfiguraci.
		/// @param component - konfigurace komponenty 
		/// @return dočasný soubor reprezentující vytvořený zip archiv
		TMP_FILE create_zip(cr<ITEM> component)
		{
			path json_path = "oul.component.json";
			TMP_FILE json(json_path.string(), true);
			ofstream out = json.write();
			save_json(component, out);
			out.close();

			path json_dir = json.get_directory();
			TMP_FILE zip_file(json_dir / "komponent.zip");
			zip(json_dir, zip_file, json.get_path());

			return zip_file;
		}

	protected:
		path zip_path;
		path unzip_path;

		ABSTRACT_ZIP_TOOL(cr<path> zip_path, cr<path> unzip_path):
			zip_path(zip_path), unzip_path(unzip_path)
		{}

		/// @brief Spustí zipovací nástroj.
		/// @tparam ...T - std::string nebo boost::filesystem::path
		/// @param tool_path - cesta nástroje
		/// @param working_directory - složka, ve které se nástroj spustí
		/// @param ...args - argumenty předané nástroji
		template <CmdArgs ...T>
		void call_tool(cr<path> tool_path, cr<path> working_directory, T... args)
		{
			child ch(tool_path, args..., start_dir(working_directory), std_out > null, std_err > null);
			ch.wait();
			if (ch.exit_code() != 0)
			{
				throw ZippingError(zipping_error);
			}
		}
		/// @brief Spustí zipovací nástroj v aktuálním pracovním adresáři.
		/// @tparam ...T - std::string nebo boost::filesystem::path 
		/// @param tool_path - cesta nástroje 
		/// @param ...args - argumenty předané nástroji 
		template <CmdArgs ...T>
		void call_tool_here(cr<path> tool_path, T... args)
		{
			call_tool(tool_path, current_path(), args...);
		}

		/// @brief Přidá daný soubor do zadaného arhivu.
		/// @param working_dir - pracovní složka, vůči které je soubor do archivu vložen (ovlivňuje cestu souboru uvnitř archivu)
		/// @param zip_file - cesta k archivu
		/// @param entry - soubor přidávaný do archivu
		virtual void zip(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) = 0;
		/// @brief Rozbalí daný archiv do zadané složky.
		/// @param working_dir - pracovní složka, do které je archiv rozbalen
		/// @param zip_file - cesta k archivu
		virtual void unzip(cr<path> working_dir, cr<TMP_FILE> zip_file) = 0;
	public:
		/// @brief Vytvoří zip archiv a přidá do něj všechny soubory dané komponenty (včetně konfigurace).
		/// @param base - kořen projektu
		/// @param component - konfigurace komponenty
		void zip(cr<path> base, cr<ITEM> component)
		{
			path component_dir = base / component.location;
			TMP_FILE zip_file = create_zip(component);
			FILE_ITERATOR it = FILE_ITERATOR::init(component_dir, component.include, component.exclude);

			for (cr<path> entry : it)
			{
				zip(component_dir, zip_file, entry);
			}
		}
		virtual ~ABSTRACT_ZIP_TOOL() = default;
	};
}
