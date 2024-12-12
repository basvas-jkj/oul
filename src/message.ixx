export module message;

import <iostream>;
import <string>;
import <map>;

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
	const string config_not_found = "Configuration file not found.\nCall 'oul init' or move to an initialized project.";
	const string missing_component_name = "Missing component name.\nCall this command with one additional argument.";
	
}

export enum class CONFIG_ERR
{
	root_not_object, missing_project_name, components_not_array, url_not_string
};
using enum CONFIG_ERR;
export const map<CONFIG_ERR, string> configuration_errors
{
	{root_not_object, "Root of the configuration shall be an object."}, 
	{missing_project_name, "Name of the project is missing in the configuration."},
	{components_not_array, "List of components is mandatory has to be an array."},
	{url_not_string, "Url has to be a string."}
};