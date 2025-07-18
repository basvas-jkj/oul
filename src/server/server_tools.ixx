module;

#include <boost/filesystem.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

export module server:tools;
export import :base;
import zip;
import tools;
import support;
import temporary_file;

namespace fs = boost::filesystem;
namespace oul
{
	string last_path_item(cr<string> s)
	{
		fs::path p(s);
		return p.filename().string();
	}

	/// @brief Reprezentuje nástroj SPC.
	/// (http://www.openssh.com/)
	export class SCP: public CLIENT
	{
		string upload_url;
		string download_url;

		/// @brief Nahraje komponentu na server.
		/// @param from - složka, ze které má být komponenta nahrána
		void upload_component(cr<fs::path> from)
		{
			call_client("-r", from, upload_url);
		}
		/// @brief Stáhne komponentu ze serveru.
		/// @param where - složka, do které má být komponenta stažena
		void download_component(cr<fs::path> where)
		{
			call_client("-r", download_url, where);
		}

	public:
		/// @param component_name - jméno komponenty, kterou klient spojuje
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		SCP(cr<string> component_name, cr<string> url, cr<fs::path> cl):
			CLIENT(component_name, cl, "scp")
		{
			upload_url = url;
			download_url = url_append(url, component_name);
		}
		/// @brief Odešle komponentu na server.
		/// @param component - konfigurace odesílané komponenty
		void upload(cr<ITEM> component) override
		{
			TMP_FOLDER folder(component_name, true);
			fs::path tmp_dir = folder.get_name();

			save_config(component, tmp_dir / "oul.component.json");
			copy_component(component, component_location, tmp_dir);
			upload_component(tmp_dir);
		}
		/// @brief Stáhne komponentu ze serveru.
		/// @return konfigurace stažené komponenty
		ITEM download() override
		{
			TMP_FOLDER folder(component_name, true);
			fs::path tmp_dir = folder.get_name();

			download_component(tmp_dir);
			ITEM component = load_config(tmp_dir / component_name / "oul.component.json");
			copy_component(component, tmp_dir / component_name, component_location);

			return component;
		}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		vector<string> list_components() override
		{
			TMP_FOLDER folder(component_name, true);
			fs::path tmp_dir = folder.get_name();

			download_component(tmp_dir);

			fs::path base = tmp_dir / last_path_item(download_url);
			vector<string> list;
			fs::directory_iterator it(base);
			for (cr<fs::directory_entry> entry : it)
			{
				if (is_component(entry))
				{
					list.push_back(entry.path().filename().generic_string());
				}
			}

			return list;
		}
	};
	/// @brief Reprezentuje nástroj CURL pro použití protokolu HTTP.
	/// (https://curl.se/)
	export class CURL_HTTP: public CLIENT
	{
		string url;
		zip_tool_ptr tool;

		/// @brief Odešle jeden soubor na server.
		/// @param url - url serveru
		/// @param file - cesta k odesílanému souboru
		void upload_file(cr<string> url, cr<fs::path> file)
		{
			string post_argument = format("@{}", file.generic_string());
			call_client("--data-binary", post_argument, url);
		}
		/// @brief Stáhne jeden soubor ze serveru.
		/// @param url - url serveru
		/// @param where - cesta, na kterou se má soubor stáhnout
		void download_file(cr<string> url, cr<fs::path> file)
		{
			call_client("-o", file, url);
		}

	public:
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		CURL_HTTP(cr<string> name, cr<string> url, cr<fs::path> cl): CLIENT(name, cl, "curl")
		{
			this->url = url_append(url, name);
			tool = select_zip_tool();
		}
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
	/// @brief Reprezentuje nástroj CURL pro použití protokolu FTP.
	/// (https://curl.se/)
	export class CURL_FTP: public CLIENT
	{
		string url;
		zip_tool_ptr tool;

		/// @brief Odešle jeden soubor na server.
		/// @param url - url serveru
		/// @param file - cesta k odesílanému souboru
		void upload_file(cr<string> url, cr<fs::path> file)
		{
			call_client("-T", file, url);
		}
		/// @brief Stáhne jeden soubor ze serveru.
		/// @param url - url serveru
		/// @param where - cesta, na kterou se má soubor stáhnout
		void download_file(cr<string> url, cr<fs::path> file)
		{
			call_client("-o", file, url);
		}

	public:
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		CURL_FTP(cr<string> name, cr<string> url, cr<fs::path> cl): CLIENT(name, cl, "curl")
		{
			this->url = url_append(url, name);
			tool = select_zip_tool();
		}
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
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		vector<string> list_components() override
		{
			istringstream output;

			try
			{
				output = istringstream(call_client_read_output("--list-only", url));
			}
			catch (...)
			{
				throw ClientError();
			}

			vector<string> list;
			while (output.good())
			{
				string line;
				getline(output, line);

				if (line == "" || line == "\r")
				{
					continue;
				}
				else if (line.ends_with('\r'))
				{
					line.pop_back();
				}

				list.push_back(std::move(line));
			}
			return list;
		}
	};
	/// @brief Reprezentuji lokálního klienta (serverem se myslí lokální složka).
	export class LOCAL: public CLIENT
	{
		string url;

	public:
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param url - url, se kterou je klient spojený (v tomto případě cesta k lokální složce)
		/// @param cl - umístění komponenty, kterou klient spravuje
		LOCAL(cr<string> name, cr<string> url, cr<fs::path> cl): CLIENT(name, cl)
		{
			this->url = url_append(url, component_name);
		}
		/// @brief Odešle komponentu na server.
		/// @param component - konfigurace odesílané komponenty
		void upload(cr<ITEM> component) override
		{
			save_config(component, fs::path(url) / "oul.component.json");
			copy_component(component, component_location, url);
		}
		/// @brief Stáhne komponentu ze serveru.
		/// @return konfigurace stažené komponenty
		ITEM download() override
		{
			ITEM component = load_config(fs::path(url) / "oul.component.json");
			copy_component(component, url, component_location);

			return component;
		}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		vector<string> list_components() override
		{
			vector<string> list;
			fs::directory_iterator it(url);
			for (cr<fs::directory_entry> entry : it)
			{
				if (is_component(entry))
				{
					list.push_back(entry.path().filename().generic_string());
				}
			}
			return list;
		}
	};
}
