module;

#include <boost/filesystem.hpp>

export module zip:tools;
import :base;

namespace oul
{
	/// @brief Reprezentuje nástroje zip a unzip.
	/// (https://infozip.sourceforge.net/)
	class TOOL_ZIP: public ABSTRACT_ZIP_TOOL
	{
	public:
		/// @param zip_path - cesta k nástroji zip
		/// @param unzip_path - cesta k nástroji unzip
		TOOL_ZIP(cr<path> zip_path, cr<path> unzip_path): ABSTRACT_ZIP_TOOL(zip_path, unzip_path) {}

		/// @brief Přidá daný soubor do zadaného arhivu.
		/// @param working_dir - pracovní složka, vůči které je soubor do archivu vložen (ovlivňuje
		/// cestu souboru uvnitř archivu)
		/// @param zip_file - cesta k archivu
		/// @param entry - soubor přidávaný do archivu
		void add_file(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) override
		{
			bool success = call_tool(zip_path, working_dir, zip_file.get_path(), entry);
			if (!success)
			{
				throw new ZippingError(ERROR::zipping_error);
			}
		}
		/// @brief Rozbalí daný archiv do zadané složky.
		/// @param working_dir - pracovní složka, do které je archiv rozbalen
		/// @param zip_file - cesta k archivu
		void extract_files(cr<path> working_dir, cr<TMP_FILE> zip_file) override
		{
			create_directories(working_dir);
			bool success = call_tool(unzip_path, working_dir, zip_file.get_path());
			if (!success)
			{
				throw new ZippingError(ERROR::zipping_error);
			}
		}
	};
	/// @brief Reprezentuje nástroje 7-zip
	/// (https://www.7-zip.org/)
	class TOOL_7Z: public ABSTRACT_ZIP_TOOL
	{
	public:
		/// @param p - cesta k nástroji 7-zip
		TOOL_7Z(cr<path> p): ABSTRACT_ZIP_TOOL(p, p) {}

		/// @brief Přidá daný soubor do zadaného arhivu.
		/// @param working_dir - pracovní složka, vůči které je soubor do archivu vložen (ovlivňuje
		/// cestu souboru uvnitř archivu)
		/// @param zip_file - cesta k archivu
		/// @param entry - soubor přidávaný do archivu
		void add_file(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) override
		{
			bool success = call_tool(zip_path, working_dir, "a", zip_file.get_path(), entry);
			if (!success)
			{
				throw new ZippingError(ERROR::zipping_error);
			}
		}
		/// @brief Rozbalí daný archiv do zadané složky.
		/// @param working_dir - pracovní složka, do které je archiv rozbalen
		/// @param zip_file - cesta k archivu
		void extract_files(cr<path> working_dir, cr<TMP_FILE> zip_file) override
		{
			create_directories(working_dir);
			bool success = call_tool(unzip_path, working_dir, "x", zip_file.get_path());
			if (!success)
			{
				throw new ZippingError(ERROR::zipping_error);
			}
		}
	};
}
