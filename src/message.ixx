export module message;

import std;
import usings;

using namespace std;

export void report_error(cr<string> message)
{
	println(cerr, "{}", message);
}
export namespace oul::message
{
	const string missing_command = "Missing command. Run 'oul help' to get a list of supported commands.";
	const string unknown_command = "Unknown command. Run 'oul help' to get a list of supported commands.";
	const string missing_group_subcommand = "'oul group' requires subcommand ('add' or 'remove').";
	const string config_found = "Configuration file has been created.";
	const string config_not_found = "Configuration file not found.\nCall 'oul init' or move to an initialized project.";
	const string missing_component_name = "Missing component name.\nCall this command with one additional argument.";
	const string component_already_exists = "Component of this name already exists.\nCall this command with different name or rename original component.";

	const string component_not_found = "Component of this name doesn't exist.";
	const string file_not_exist = "File included into component has to exist.";
	const string empty_file_list = "Missing file list.";
	const string group_not_found = "Group of this name doesn't exist in selected komponent.";
	const string missing_group_name = "Missing group name.";
}

export enum class CONFIG_ERR
{
	root_not_object, missing_project_name, components_not_array, url_not_string,
	invalid_component, missing_component_name, invalid_original_name, missing_location, 
	missing_include, invalid_exclude
};
using enum CONFIG_ERR;
export const map<CONFIG_ERR, string> configuration_errors
{
	{root_not_object, "Root of the configuration shall be an object."}, 
	{missing_project_name, "Name of the project is missing in the configuration."},
	{components_not_array, "List of components is mandatory and has to be an array."},
	{url_not_string, "Url has to be a string."},
	{invalid_component, "All items in component list have to be a map."},
	{missing_component_name, "Name of some components is missing or is not a string."},
	{invalid_original_name, "Original name of component has to be a string."},
	{missing_location, "Component location is missing or is not a string."},
	{missing_include, "Component include list is missing or is not a map of file lists."},
	{invalid_exclude, "Component exclude list is not a map of file lists."}
};