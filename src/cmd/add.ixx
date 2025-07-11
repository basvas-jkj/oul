module;

#include <span>
#include <string>

export module argument_parser:add;

import component_manager;
import :command;

namespace oul::args
{
	/// @brief Reprezentuje příkaz add - zahrnutí souborů do komponenty.
	export class ADD: public COMMAND
	{
	public:
		/// @brief Zkontroluje argumenty příkazové řádky předané programu OUL.
		/// @return <code>true</code>, pokud argumenty byly předány správně, jinak
		/// <code>false</code>
		bool check() const override
		{
			if (arguments.size() < 1)
			{
				report_error(MESSAGE::missing_component_name);
			}
			if (arguments.size() < 2)
			{
				report_error(MESSAGE::missing_group_name);
			}
			if (arguments.size() < 3)
			{
				report_error(MESSAGE::empty_file_list);
				return false;
			}
			else
			{
				return true;
			}
		}
		/// @brief Spustí příkaz add programu OUL.
		void run() const override
		{
			COMPONENT_MANAGER manager = open_manager();
			span<const string> files(arguments.begin() + 2, arguments.end());
			manager.add_files(arguments[0], arguments[1], files);
		}
	};
}
