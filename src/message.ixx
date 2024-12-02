export module message;

import <string>;

using namespace std;

export namespace oul::message
{
	const string missing_command = "Missing command. Run 'oul help' to get a list of supported commands.";
	const string unknown_command = "Unknown command. Run 'oul help' to get a list of supported commands.";
	const string missing_group_subcommand = "'oul group' requires subcommand ('add' or 'remove').";

}