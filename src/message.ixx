module;

#define _CRT_SECURE_NO_DEPRECATE
#include <boost/algorithm/string.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdlib.h>

export module message;

import item;
import support;

using namespace std;
using namespace nlohmann;
using namespace boost::dll;
using boost::algorithm::to_lower;
namespace oul
{
	/// @brief Třída reprezentující chybové hlášky načtené ze souboru.
	class DICTIONARY
	{
		json object;

	public:
		/// @brief Konstruktor vytvářející prázdné objekty typu <code>DICTIONARY</code>.
		DICTIONARY() {}
		/// @brief Konstruktor načítají chybové hlášky ze souboru.
		/// @param path - Cesta k souboru, ve kterém jsou chybové hlášky uloženy.
		DICTIONARY(cr<fs::path> path)
		{
			ifstream input(path.string());
			object = json::parse(input);
		}
		/// @brief Vrátí text chybové hlášky.
		/// @param error_name - jméno chybové hlášky
		/// @return text chybové hlášky odpovídající zadanému jménu
		cr<string> operator[](cr<string> error_name)
		{
			return object[error_name].get_ref<string&>();
		}
	};

	/// @brief Načtené zprávy.
	static DICTIONARY messages;
	/// @brief Načte chybové hlášky ze souboru.
	/// @param oul_path - cesta k spustitelnému souboru oul
	export void init_messages()
	{
		string language;
		char* lang = getenv("LANG");
		if (lang != NULL)
		{
			language = string(lang);
		}

		to_lower(language);

		fs::path oul_path = program_location();
		if (language == "czech" || language.starts_with("cs_cz"))
		{
			oul_path.replace_filename("czech.json");
		}
		else
		{
			oul_path.replace_filename("english.json");
		}
		messages = DICTIONARY(oul_path);
	}

	/// @brief Výčet všech chybových a jiných hlášek.
	export enum class MESSAGE
	{
		unexpected_error,
		component_list,
		component_server_list,
		local_only,
		help,

		// init prompts
		init_name,
		init_format,
		init_url,
		init_json_fallback,
		init_yaml_fallback,

		// help messages
		help_add,
		help_create,
		help_delete,
		help_exclude,
		help_group_add,
		help_group_remove,
		help_help,
		help_init,
		help_install,
		help_list,
		help_server_list,
		help_remove,
		help_rename,
		help_upload,

		// comand line errors
		missing_command,
		unknown_command,
		missing_group_subcommand,
		missing_component_name,
		component_already_exists,
		component_not_found,
		missing_group_name,
		group_already_exists,
		group_not_found,
		empty_file_list,
		file_outside_component,
		too_much_arguments,
		unknown_option,

		// configuration errors
		config_found,
		config_not_found,
		invalid_configuration_format,

		root_not_object,
		missing_project_name,
		components_not_array,
		url_not_string,
		invalid_component,
		invalid_component_name,
		invalid_original_name,
		missing_location,
		missing_include,
		invalid_exclude,

		// external tools
		missing_client,
		client_error,
		unknown_client,
		missing_zip_tool,
		zipping_error,

		// server errors
		missing_url
	};
	using enum MESSAGE;

	/// @brief Objekt zobrazující kódy chybových hlášek na jména.
	static map<MESSAGE, string> error_list{
		{unexpected_error, "unexpected_error"},
		{component_list, "component_list"},
		{component_server_list, "component_server_list"},
		{local_only, "local_only"},
		{help, "help"},

		{init_name, "init_name"},
		{init_format, "init_format"},
		{init_url, "init_url"},
		{init_json_fallback, "init_json_fallback"},
		{init_yaml_fallback, "init_yaml_fallback"},

		{help_add, "help_add"},
		{help_create, "help_create"},
		{help_delete, "help_delete"},
		{help_exclude, "help_exclude"},
		{help_group_add, "help_group_add"},
		{help_group_remove, "help_group_remove"},
		{help_help, "help_help"},
		{help_init, "help_init"},
		{help_install, "help_install"},
		{help_list, "help_list"},
		{help_server_list, "help_server_list"},
		{help_remove, "help_remove"},
		{help_rename, "help_rename"},
		{help_upload, "help_upload"},

		{missing_command, "missing_command"},
		{unknown_command, "unknown_command"},
		{missing_group_subcommand, "missing_group_subcommand"},

		{missing_component_name, "missing_component_name"},
		{component_already_exists, "component_already_exists"},
		{component_not_found, "component_not_found"},

		{missing_group_name, "missing_group_name"},
		{group_already_exists, "group_already_exists"},
		{group_not_found, "group_not_found"},

		{empty_file_list, "empty_file_list"},
		{file_outside_component, "file_outside_component"},
		{too_much_arguments, "too_much_arguments"},
		{unknown_option, "unknown_option"},

		{config_found, "config_found"},
		{config_not_found, "config_not_found"},
		{invalid_configuration_format, "invalid_configuration_format"},

		{root_not_object, "root_not_object"},
		{missing_project_name, "missing_project_name"},
		{components_not_array, "components_not_array"},
		{url_not_string, "url_not_string"},
		{invalid_component, "invalid_component"},
		{invalid_component_name, "invalid_component_name"},
		{invalid_original_name, "invalid_original_name"},
		{missing_location, "missing_location"},
		{missing_include, "missing_include"},
		{invalid_exclude, "invalid_exclude"},

		{missing_client, "missing_client"},
		{client_error, "client_error"},
		{unknown_client, "unknown_client"},
		{missing_zip_tool, "missing_zip_tool"},
		{zipping_error, "zipping_error"},

		{missing_url, "missing_url"}};

	/// @brief Vypíše výzvu na standardní výstup.
	/// @param name - kód zprávy
	export void print_init_prompt(MESSAGE name)
	{
		cr<string> error = error_list[name];
		print(cout, "{}", messages[error]);
	}
	/// @brief Vypíše chybovou hlášku na standardní chybový výstup.
	/// @param name - kód chybové hlášky
	export void report_error(MESSAGE name)
	{
		cr<string> error = error_list[name];
		println(cerr, "{}", messages[error]);
	}
	/// @brief Vypíše chybovou hlášku doplněnou o hodnotu argumentu na standardní chybový výstup.
	/// @param name - kód chybové hlášky
	/// @param arg - argument, která
	export void report_error(MESSAGE name, cr<vector<string>> args)
	{
		cr<string> error = error_list[name];
		string message = messages[error];

		size_t index = 0;
		for (cr<string> arg : args)
		{
			index = message.find('*', index);
			if (index == (size_t)-1)
				break;
			message.replace(index, 1, arg);
			index += arg.size();
		}

		println(cerr, "{}", message);
	}
	/// @brief Vypíše identifikátor verze.
	export void print_version()
	{
		println(cout, "oul {}", version_id);
	}
	/// @brief Vypíše seznam komponent.
	/// @param components - seznam komponent k vypsání
	export void print_component_list(cr<vector<ITEM>> components)
	{
		cr<string> component_list_text = error_list[component_list];
		println(cout, "{}", messages[component_list_text]);
		for (cr<ITEM> i : components)
		{
			cr<string> local_only_text = error_list[local_only];
			cr<string> url = (i.url.empty()) ? messages[local_only_text] : i.url;
			println(cout, "\t{}:\t{}\t({})", i.name, i.location, url);
		}
	}
	/// @brief Vypíše seznam komponent ze serveru.
	/// @param components - seznam komponent k vypsání
	/// @param url - url serveru
	export void print_component_list(cr<vector<string>> components, cr<string> url)
	{
		report_error(component_server_list, {url});
		for (cr<string> name : components)
		{
			println(cout, "\t{}", name);
		}
	}
	/// @brief Vypíše nápovědu programu.
	export void print_help()
	{
		vector commands = {help_add,	   help_create,		  help_delete, help_exclude,
						   help_group_add, help_group_remove, help_help,   help_init,
						   help_install,   help_server_list,  help_list,   help_remove,
						   help_rename,	   help_upload};

		cr<string> help_text = error_list[help];
		println(cout, "{}", messages[help_text]);
		for (auto&& command : commands)
		{
			cr<string> command_help_text = error_list[command];
			println(cout, "\t{}", messages[command_help_text]);
		}
	}

	/// @brief Základní třída pro výjimky použité v tomto projektu.
	export class CommonException: exception
	{
		MESSAGE name;

	public:
		/// @brief Konstruktor vytvářející objekty <code>CommonException</code>
		/// @param n - kód chybové hlášky
		CommonException(MESSAGE n): name(n) {}
		/// @brief Vypíše hlášku výjimky na standarní chybový výstup.
		void report() const
		{
			report_error(name);
		}
		/// @brief Vrátí kód chyby, kterou tato výjimka oznamuje.
		/// @return - kód chybové hlášky
		MESSAGE get_error_code()
		{
			return name;
		}
	};
	export class ArgumentException: exception
	{
		MESSAGE name;
		vector<string> arguments;

	public:
		/// @brief Konstruktor vytvářející objekty <code>ArgumentException</code>
		/// @param n - kód chybové hlášky
		/// @param arg - argument chybové hlášky
		ArgumentException(MESSAGE n, cr<vector<string>> args): name(n), arguments(args) {}
		/// @brief Vypíše hlášku výjimky na standarní chybový výstup.
		void report() const
		{
			report_error(name, arguments);
		}
	};
}
