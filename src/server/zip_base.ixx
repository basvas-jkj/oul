module;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

export module zip:base;

import tools;
import support;
import message;
import file_iterator;
import configuration;
import temporary_file;

using namespace std;
using namespace boost::filesystem;

namespace oul
{
	/// @brief Chyba: nenalezen žádný zipovací nástroj.
	export class MissingZipTool: public CommonException
	{
	public:
		MissingZipTool(MESSAGE name): CommonException(name) {}
	};
	/// @brief Chyba: vytváření nebo rozbalování archovu selhalo
	export class ZippingError: public CommonException
	{
	public:
		ZippingError(MESSAGE name = MESSAGE::zipping_error): CommonException(name) {}
	};

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
			add_file(json_dir, zip_file, json.get_name());

			return zip_file;
		}

	protected:
		path zip_path;
		path unzip_path;

		/// @brief Základní konstruktor objektů reprezentujících zipové nástroje.
		/// @param zip_path - cesta k nástroji pro vytváření archivu
		/// @param unzip_path - cesta k nástroji pro rozbalení archivu
		ABSTRACT_ZIP_TOOL(cr<path> zip_path, cr<path> unzip_path):
			zip_path(zip_path), unzip_path(unzip_path)
		{
		}

		/// @brief Přidá daný soubor do zadaného arhivu.
		/// @param working_dir - pracovní složka, vůči které je soubor do archivu vložen (ovlivňuje
		/// cestu souboru uvnitř archivu)
		/// @param zip_file - cesta k archivu
		/// @param entry - soubor přidávaný do archivu
		virtual void add_file(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) = 0;
		/// @brief Rozbalí daný archiv do zadané složky.
		/// @param working_dir - pracovní složka, do které je archiv rozbalen
		/// @param zip_file - cesta k archivu
		virtual void extract_files(cr<path> working_dir, cr<TMP_FILE> zip_file) = 0;

	public:
		/// @brief Vytvoří zip archiv a přidá do něj všechny soubory dané komponenty (včetně
		/// konfigurace).
		/// @param component - konfigurace komponenty
		/// @param component_location - umístění komponenty
		/// @return objekt reprezentující dočasný soubor zipového archivu
		TMP_FILE zip(cr<ITEM> component, cr<string> component_location)
		{
			TMP_FILE zip_file = create_zip(component);
			FILE_ITERATOR it(component_location, component.include, component.exclude);

			for (cr<path> entry : it)
			{
				add_file(component_location, zip_file, entry);
			}

			return zip_file;
		}
		/// @brief Rozbalí zip archiv.
		/// @param zip_file - zip archiv
		/// @param component_location - umístění komponenty
		/// @return konfigurace rozbalené komponenty
		ITEM unzip(cr<TMP_FILE> zip_file, cr<string> component_location)
		{
			extract_files(component_location, zip_file);

			path name;
			if (name = component_location / "oul.component.json"; exists(name))
				;
			else if (name = component_location / "oul.component.yaml"; exists(name))
				;
			else
			{
				throw ZippingError();
			}

			ifstream component_file(name.string());
			auto&& component = load_component(component_file);
			component_file.close();

			remove(name);
			return ITEM(component);
		}
		virtual ~ABSTRACT_ZIP_TOOL() = default;
	};
}
