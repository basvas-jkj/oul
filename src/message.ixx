export module message;

import <iostream>;
import <string>;

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

	
}