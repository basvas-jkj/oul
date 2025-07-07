module;

#include <boost/filesystem.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>
#undef ERROR

export module server:tools;
export import :base;
import zip;
import tools;
import support;
import temporary_file;

namespace fs = boost::filesystem;
namespace oul
{
	/// @brief Reprezentuje nástroj SPC.
	/// (http://www.openssh.com/)
	export class SCP: public CLIENT
	{
	protected:
		/// @brief Odešle jeden soubor na server.
		/// @param url - url serveru
		/// @param file - cesta k odesílanému souboru
		void upload_file(cr<string> url, cr<fs::path> file) override
		{
			call_client(url, file);
		}
		/// @brief Stáhne jeden soubor ze serveru.
		/// @param url - url serveru
		/// @param where - cesta, na kterou se má soubor stáhnout
		void download_file(cr<string> url, cr<fs::path> file) override
		{
			string output = call_client_read_output(file, url);
		}

	public:
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		SCP(cr<string> url, cr<fs::path> base): CLIENT(url, base, "scp") {}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		vector<string> list_components() override
		{
			return vector<string>();
		}
	};
	/// @brief Reprezentuje nástroj CURL.
	/// (https://curl.se/)
	export class CURL: public CLIENT
	{
		zip_tool_ptr tool;

	protected:
		/// @brief Odešle jeden soubor na server.
		/// @param url - url serveru
		/// @param file - cesta k odesílanému souboru
		void upload_file(cr<string> url, cr<fs::path> file) override
		{
			string post_argument = format("@{}", file.generic_string());
			call_client("--data-binary", post_argument, url);
		}
		/// @brief Stáhne jeden soubor ze serveru.
		/// @param url - url serveru
		/// @param where - cesta, na kterou se má soubor stáhnout
		void download_file(cr<string> url, cr<fs::path> file) override
		{
			call_client("-o", file, url);
		}

	public:
		/// @brief Zazipuje komponentu a odešle ji na server.
		/// @param component - konfigurace odesílané komponenty
		void upload(cr<ITEM> component) override
		{
			TMP_FILE file = tool->zip(component, component_location.string());
			upload_file(url, file.get_path());
		}
		/// @brief Stáhne komponentu ze serveru jako zipový archiv a rozbalí ji.
		/// @return konfigurace stažené komponenty
		ITEM download() override
		{
			TMP_FILE file("%%%%-%%%%-%%%%-%%%%.zip", false);
			download_file(url, file.get_path());
			return tool->unzip(file, component_location.string());
		}
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		CURL(cr<string> url, cr<fs::path> base): CLIENT(url, base, "curl")
		{
			tool = select_zip_tool();
		}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		vector<string> list_components() override
		{
			string output = call_client_read_output(url);
			Node list = Load(output);
			try
			{
				return list.as<vector<string>>();
			}
			catch (YAML::BadConversion&)
			{
				throw ClientError();
			}
		}
	};
	/// @brief Reprezentuji lokálního klienta (serverem se myslí lokální složka).
	export class LOCAL: public CLIENT
	{
	private:
		/// @brief
		/// @param entry -
		/// @return <code>true</code>, pokud položka reprezentuje komponentu, <code>false</code> v
		/// opačném případě
		bool is_component(cr<fs::directory_entry> entry)
		{
			if (!entry.is_directory())
			{
				return false;
			}
			fs::path config_file = entry.path() / "oul.component.json";
			return is_regular_file(config_file);
		}

	protected:
		/// @brief Odešle jeden soubor na server.
		/// @param url - url serveru
		/// @param file - cesta k odesílanému souboru
		void upload_file(cr<string> url, cr<fs::path> file) override
		{
			fs::path entry = fs::relative(file, component_location);
			copy_file(component_location, url, entry);
		}
		/// @brief Stáhne jeden soubor ze serveru.
		/// @param url - url serveru
		/// @param where - cesta, na kterou se má soubor stáhnout
		void download_file(cr<string> url, cr<fs::path> file) override
		{
			fs::path entry = fs::relative(file, url);
			copy_file(url, component_location, entry);
		}

	public:
		/// @param url - url, se kterou je klient spojený (v tomto případě cesta k lokální složce)
		/// @param cl - umístění komponenty, kterou klient spravuje
		LOCAL(cr<string> url, cr<fs::path> base): CLIENT(url, base) {}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		vector<string> list_components() override
		{
			vector<string> components;
			fs::directory_iterator it(url);
			for (cr<fs::directory_entry> entry : it)
			{
				if (is_component(entry))
				{
					components.push_back(entry.path().filename().generic_string());
				}
			}
			return components;
		}
	};
}
