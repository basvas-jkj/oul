module;

#include <span>
#include <string>

export module args:exclude;

import component_manager;
import message;
import :command;

using namespace std;

namespace oul::args
{
	/// @brief Reprezentuje příkaz exclude - vyčlenění souborů z komponenty.
	export class EXCLUDE: public COMMAND
	{
	public:
		/// @brief Zkontroluje argumenty příkazové řádky předané programu OUL.
		/// @return <code>true</code>, pokud argumenty byly předány správně, jinak
		/// <code>false</code>
		bool check() const override
		{
			if (arguments.size() < 1)
			{
				report_error(ERROR::missing_component_name);
			}
			if (arguments.size() < 2)
			{
				report_error(ERROR::missing_group_name);
			}
			if (arguments.size() < 3)
			{
				report_error(ERROR::empty_file_list);
				return false;
			}
			else
			{
				return true;
			}
		}
		/// @brief Spustí příkaz exclude programu OUL.
		void run() const override
		{
			COMPONENT_MANAGER manager = open_manager();
			span<const string> files(arguments.begin() + 2, arguments.end());
			manager.exclude_files(arguments[0], arguments[1], files);
		}
	};
}
