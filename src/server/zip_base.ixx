module;

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#undef ERROR

export module zip:base;

import tmp;
import tools;
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
		ZippingError(ERROR name = zipping_error): CommonException(name)
		{}
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
			zip(json_dir, zip_file, json.get_path());

			return zip_file;
		}

	protected:
		path zip_path;
		path unzip_path;

		ABSTRACT_ZIP_TOOL(cr<path> zip_path, cr<path> unzip_path):
			zip_path(zip_path), unzip_path(unzip_path)
		{}

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
		/// @param component - konfigurace komponenty
		/// @param component_location - umístění komponenty
		TMP_FILE zip(cr<ITEM> component, cr<string> component_location)
		{
			TMP_FILE zip_file = create_zip(component);
			FILE_ITERATOR it = FILE_ITERATOR::init(component_location, component.include, component.exclude);

			for (cr<path> entry : it)
			{
				zip(component_location, zip_file, entry);
			}

			return zip_file;
		}
		ITEM unzip(cr<TMP_FILE> zip_file, cr<string> component_location)
		{
			unzip(component_location, zip_file);

			path name;
			if (name = component_location / "oul.component.json"; exists(name));
			else if (name = component_location / "oul.component.yaml"; exists(name));
			else
			{
				throw ZippingError();
			}

			ITEM component(load_component(name.string()));
			return component;
		}
		virtual ~ABSTRACT_ZIP_TOOL() = default;
	};
}
