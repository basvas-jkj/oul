module;
#include <map>
#include <optional>
#include <string>
#include <vector>

export module args:command;

import usings;
import common;
import message;
import component_manager;

using namespace std;

namespace oul
{
	/// @brief Objekt zobrazující dlouhé přepínače na krátké.
	const map<string, string> short_variants = {{"-version", "-v"}, {"-where", "-w"}};
	/// @brief Chyba: konfigurace nenalezena.
	export class ConfigurationNotFound: public CommonException
	{
	public:
		ConfigurationNotFound(ERROR name): CommonException(name) {}
	};
	/// @brief Třída ukládající přepínače a jejich argumenty předané jako argumenty příkazové řádky.
	export class OPTIONS
	{
		map<string, string> options;
		vector<string> supported_options{"-as", "-url", "-v", "-version", "-w", "-where"};

	public:
		/// @brief Přidá přepínač a jeho arugument.
		/// @param name - jméno přepínače
		/// @param value - volitelná hodnota argumentu
		void add(cr<string> name, cr<string> value = "")
		{
			if (ranges::find(supported_options, name) == supported_options.end())
			{
				throw ArgumentException(ERROR::unknown_option, {name});
			}

			auto it = short_variants.find(name);

			if (it == short_variants.end())
			{
				options[name] = value;
			}
			else
			{
				cr<string> short_name = it->second;
				options[short_name] = value;
			}
		}
		/// @brief Dotaz, zda byl program spuštěn s daným přepínačem.
		/// @param name - jméno dotazovaného přepínače
		/// @return <code>true</code>, pokud byl program spuštěn s daným přepínačem, jinak
		/// <code>false</code>
		bool includes(cr<string> name) const
		{
			return options.find(name) != options.end();
		}
		/// @brief Vrátí argument daného přepínače.
		/// @param name - jméno dotazovaného přepínače
		/// @return hodnotu argumentu, pokud byl program spuštěn s daným přepínačem, jinak prázdný
		/// řetezec
		cr<string> get(cr<string> name) const
		{
			auto it = options.find(name);
			if (it == options.end())
			{
				static string empty = "";
				return empty;
			}
			else
			{
				return it->second;
			}
		}
	};
	/// @brief Základní třída pro reprezentaci příkazů programu OUL.
	export class COMMAND
	{
	protected:
		OPTIONS opt;
		vector<string> arguments;

		/// @brief Najde konfiguračnímu souboru programu OUL.
		/// @return cesta ke konfiguračnímu souboru, pokud byl nalezen
		/// @throw <code>ConfigurationNotFound</code>, pokud konfigurační soubor nebyl nalezen
		static string find_configuration()
		{
			optional<string> path = CONFIG::find();
			if (path == nullopt)
			{
				throw ConfigurationNotFound(ERROR::config_not_found);
			}
			else
			{
				return *path;
			}
		}
		/// @brief Najde a načte konfigurační soubor.
		/// @return načtená konfigurace, pokud byl konfigurační soubor nalezen
		/// @throw <code>ConfigurationNotFound</code>, pokud konfigurační soubor nebyl nalezen
		static CONFIG read_configuration()
		{
			string path = find_configuration();
			return CONFIG(path);
		}
		/// @brief Načte konfiguraci a vytvoří objekt typu <code>COMPONENT_MANAGER</code>, který
		/// spravuje nalezenou konfiguraci.
		/// @return otevřený <code>COMPONENT_MANAGER</code>, pokud byl konfigurační soubor nalezen
		/// @throw <code>ConfigurationNotFound</code>, pokud konfigurační soubor nebyl nalezen
		static COMPONENT_MANAGER open_manager()
		{
			CONFIG c = read_configuration();
			COMPONENT_MANAGER manager(std::move(c));
			return manager;
		}

	public:
		virtual ~COMMAND() = default;

		/// @brief Inicializuje objekt příkazu programu OUL.
		/// @param opt - seznam předaných přepínačů
		/// @param arguments - seznam předaných pozičních argumentů
		void init(OPTIONS&& opt, vector<string>&& arguments)
		{
			this->opt = std::move(opt);
			this->arguments = std::move(arguments);

			if (this->opt.includes("-v"))
			{
				print_version();
			}
		}
		/// @brief Zkontroluje argumenty příkazové řádky předané programu OUL.
		/// @return vždy <code>true</code>
		virtual bool check() const
		{
			return true;
		}
		/// @brief Spustí příkaz programu OUL.
		virtual void run() const = 0;
	};
}
