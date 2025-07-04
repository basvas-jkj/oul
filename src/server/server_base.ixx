module;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <yaml-cpp/yaml.h>

export module server:base;
import tools;
import common;
import config;
import message;
import file_iterator;

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
		fs::path component_location;
		fs::path client_path;

		/// @brief Základní konstruktor objektů reprezentujících klienty.
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		CLIENT(cr<string> url, cr<fs::path> cl): url(url), component_location(cl) {}
		/// @brief Základní konstruktor objektů reprezentujících klienty.
		/// @param url - url, se kterou je klient spojený
		/// @param cl - umístění komponenty, kterou klient spravuje
		/// @param client_name - jméno klienta volatelné z příkazové řádky
		CLIENT(cr<string> url, cr<fs::path> cl, cr<string> client_name):
			url(url), component_location(cl)
		{
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

	public:
		/// @brief Odešle komponentu na server.
		/// @param component - konfigurace odesílané komponenty
		virtual void upload(cr<ITEM> component)
		{
			fs::create_directories(url);
			fs::path json_path = fs::path(url) / "oul.component.json";
			ofstream out(json_path.string());
			save_json(component, out);
			out.close();

			FILE_ITERATOR it(component_location, component.include, component.exclude);
			for (cr<fs::path> file : it)
			{
				upload_file(url, file);
			}
		}
		/// @brief Stáhne komponentu ze serveru.
		/// @return konfigurace stažené komponenty
		virtual ITEM download()
		{
			fs::create_directories(component_location);
			fs::path json_path = fs::path(url) / "oul.component.json";

			ifstream component_file(json_path.string());
			auto&& loaded_component = load_component(component_file, false);
			ITEM component(loaded_component);

			FILE_ITERATOR it(url, component.include, component.exclude);
			for (cr<fs::path> file : it)
			{
				download_file(url, file);
			}
			return component;
		}
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		virtual vector<string> list_components() = 0;
		virtual ~CLIENT() = default;
	};
	export using client_ptr = unique_ptr<CLIENT>;
}
