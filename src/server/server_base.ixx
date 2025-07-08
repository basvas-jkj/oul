module;

#include <boost/filesystem.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

export module server:base;
import tools;
import support;
import message;
import file_iterator;
import configuration;
import temporary_file;

using namespace std;
using namespace YAML;
namespace fs = boost::filesystem;
namespace oul
{
	/// @brief Chyba: nenalezen žádaný externí klient.
	export class MissingClient: public CommonException
	{
	public:
		MissingClient(): CommonException(ERROR::missing_client) {}
	};
	/// @brief Chyba: spuštění klienta archovu selhalo
	export class ClientError: public CommonException
	{
	public:
		ClientError(): CommonException(ERROR::client_error) {}
	};
	/// @brief Základní typ klientů.
	export class CLIENT
	{
	protected:
		string url;
		string component_name;
		fs::path component_location;
		fs::path client_path;

		/// @brief Základní konstruktor objektů reprezentujících klienty.
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		CLIENT(cr<string> component_name, cr<string> url, cr<fs::path> cl):
			url(url), component_name(component_name), component_location(cl)
		{
			url_append(this->url, component_name);
		}
		/// @brief Základní konstruktor objektů reprezentujících klienty.
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		/// @param client_name - jméno klienta volatelné z příkazové řádky
		CLIENT(cr<string> component_name, cr<string> url, cr<fs::path> cl, cr<string> client_name):
			url(url), component_name(component_name), component_location(cl)
		{
			url_append(this->url, component_name);
			client_path = find_tool(client_name);
			if (client_path == "")
			{
				throw MissingClient();
			}
		}

		/// @brief Zavolá klienta jako samostatný proces.
		/// @tparam ...T - typy argumentů převoditelné
		/// @param ...args - argumenty, se kterými je klient zavolán
		/// @throw <code>ClientError</code>, pokud volání klienta selže
		template <CmdArgs... T>
		void call_client(T&&... args)
		{
			bool success = call_tool_here(client_path, args...);
			if (!success)
			{
				throw ClientError();
			}
		}
		/// @brief Zavolá klienta jako samostatný proces.
		/// @tparam ...T - typy argumentů převoditelné
		/// @param ...args - argumenty, se kterými je klient zavolán
		/// @throw <code>ClientError</code>, pokud volání klienta selže
		template <CmdArgs... T>
		string call_client_read_output(T&&... args)
		{
			pair<bool, string> result = call_tool_read_output(client_path, args...);
			if (!result.first)
			{
				throw ClientError();
			}
			else
			{
				return result.second;
			}
		}

		/// @brief Odešle jeden soubor na server.
		/// @param url - url serveru
		/// @param file - cesta k odesílanému souboru
		virtual void upload_file(cr<string> url, cr<fs::path> file) = 0;
		/// @brief Stáhne jeden soubor ze serveru.
		/// @param url - url serveru
		/// @param where - cesta, na kterou se má soubor stáhnout
		virtual void download_file(cr<string> url, cr<fs::path> where) = 0;

		virtual fs::path copy_component(cr<ITEM> component, cr<fs::path> where) = 0;
		virtual TMP_FOLDER fetch_component(cr<string> url) = 0;
		void save_component(cr<fs::path> source, cr<fs::path> target, cr<ITEM> component)
		{
			FILE_ITERATOR it(source, component.include, component.exclude);
			for (cr<fs::path> file : it)
			{
				fs::path shifted = fs::relative(file, source);
				copy_file(source, target, shifted);
			}
		}

	public:
		/// @brief Odešle komponentu na server.
		/// @param component - konfigurace odesílané komponenty
		virtual void upload(cr<ITEM> component)
		{
			TMP_FOLDER tmp(component_name, true);

			fs::path p = copy_component(component, tmp.get_name());
			upload_file(url, p);
		}
		/// @brief Stáhne komponentu ze serveru.
		/// @return konfigurace stažené komponenty
		virtual ITEM download()
		{
			TMP_FOLDER folder = fetch_component(url);

			fs::path location = folder.get_name();
			fs::path json_path = location / "oul.component.json";
			ifstream component_file(json_path.string());

			auto&& loaded_component = load_component(component_file, false);
			fs::create_directories(component_location);

			ITEM component(loaded_component);
			save_component(location, component_location, component);

			return component;
		}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		virtual vector<string> list_components() = 0;
		virtual ~CLIENT() = default;
	};
	export using client_ptr = unique_ptr<CLIENT>;
}
