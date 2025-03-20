module;

#include <nlohmann/json.hpp>

export module message;

import std;
import usings;

using namespace std;
using namespace nlohmann;

namespace oul
{
	class DICTIONARY
	{
		json object;

	public:
		DICTIONARY()
		{
			ifstream input("english.json");
			object = json::parse(input);
		}
		cr<string> operator[](cr<string> error_name)
		{
			return object[error_name].get_ref<string&>();
		}
	};
	static DICTIONARY messages;

	export enum ERROR
	{
		// comand line errors
		missing_command, unknown_command, missing_group_subcommand, 
		missing_component_name, component_already_exists, component_not_found,
		missing_group_name, group_already_exists, group_not_found,
		file_not_exist, empty_file_list, file_outside_component,

		// configuration errors
		config_found, config_not_found,

		root_not_object, missing_project_name, components_not_array, url_not_string,
		invalid_component, invalid_component_name, invalid_original_name, missing_location,
		missing_include, invalid_exclude,

		// external tools
		missing_client, client_error,
		missing_zip_tool, zipping_error
	};
	using enum ERROR;
	
	static map<ERROR, string> error_list
	{
		{missing_command, "missing_command"},
		{unknown_command, "unknown_command"},
		{missing_group_subcommand, "missing_group_subcommand"},

		{missing_component_name, "missing_component_name"},
		{component_already_exists, "component_already_exists"},
		{component_not_found, "component_not_found"},

		{missing_group_name, "missing_group_name"},
		{group_already_exists, "group_already_exists"},
		{group_not_found, "group_not_found"},

		{file_not_exist, "file_not_exist"},
		{empty_file_list, "empty_file_list"},
		{file_outside_component, "file_outside_component"},

		{config_found, "config_found"},
		{config_not_found, "config_not_found"},

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
		{missing_zip_tool, "missing_zip_tool"},
		{zipping_error, "zipping_error"}
	};
	export void report_error(ERROR name)
	{
		cr<string> error = error_list[name];
		println(cerr, "{}", messages[error]);
	}
}
