module;

#include <boost/filesystem.hpp>
#include <fstream>
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
		MissingClient(): CommonException(MESSAGE::missing_client) {}
	};
	/// @brief Chyba: spuštění klienta archovu selhalo
	export class ClientError: public CommonException
	{
	public:
		ClientError(): CommonException(MESSAGE::client_error) {}
	};
	/// @brief Základní typ klientů.
	export class CLIENT
	{
	protected:
		string component_name;
		fs::path component_location;
		fs::path client_path;

		/// @brief Základní konstruktor objektů reprezentujících klienty.
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param cl - umístění komponenty, kterou klient spravuje
		CLIENT(cr<string> name, cr<fs::path> cl): component_name(name), component_location(cl) {}
		/// @brief Základní konstruktor objektů reprezentujících klienty.
		/// @param name - jméno komponenty, kterou klient spojuje
		/// @param cl - umístění komponenty, kterou klient spravuje
		/// @param client_name - jméno klienta volatelné z příkazové řádky
		CLIENT(cr<string> name, cr<fs::path> cl, cr<string> client_name):
			component_name(name), component_location(cl)
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
		/// @brief Zkopíruje všechny soubory komponenty.
		/// @param component - konfigurace kopírované komponenty
		/// @param source - umístění komponenty
		/// @param target - složka, do které budou soubory zkopírovány
		void copy_component(cr<ITEM> component, cr<fs::path> source, cr<fs::path> target)
		{
			FILE_ITERATOR it(source, component.include, component.exclude);
			for (cr<fs::path> file : it)
			{
				fs::path shifted = fs::relative(file, source);
				copy_file(source, target, shifted);
			}
		}
		/// @brief Uloží konfiguraci komponenty do souboru.
		/// @param component - ukládaná konfigurace
		/// @param target - cesta k souboru, do kterého bude konfigurace uložena
		void save_config(cr<ITEM> component, cr<fs::path> target)
		{
			create_directories(target.parent_path());

			ofstream component_file(target.string());
			save_json(component, component_file);
			component_file.close();
		}
		/// @brief Načte konfiguraci komponenty ze souboru.
		/// @param source - cesta k souboru, ze kterého bude konfigurace načtena
		/// @return načtená konfigurace
		ITEM load_config(cr<fs::path> source)
		{
			ifstream component_file(source.string());
			auto&& loaded_component = load_component(component_file, false);
			component_file.close();

			return ITEM(loaded_component);
		}

		/// @brief Zkontroluje, zda položka souborového systému představuje složku obsahující
		/// konfigurační soubor komponenty.
		/// @param entry - kontrolovaná položka souborového systému
		/// @return <code>true</code>, pokud položka reprezentuje komponentu, <code>false</code> v
		/// opačném případě
		bool is_component(cr<fs::directory_entry> entry)
		{
			fs::path config_file = entry.path() / "oul.component.json";
			return is_regular_file(config_file);
		}

	public:
		/// @brief Odešle komponentu na server.
		/// @param component - konfigurace odesílané komponenty
		virtual void upload(cr<ITEM> component) = 0;
		/// @brief Stáhne komponentu ze serveru.
		/// @return konfigurace stažené komponenty
		virtual ITEM download() = 0;
		/// @brief Stáhne seznam komponent ze serveru.
		/// @return Seznam komponent uložených na serveru.
		virtual vector<string> list_components() = 0;
		virtual ~CLIENT() = default;
	};
	export using client_ptr = unique_ptr<CLIENT>;
}
