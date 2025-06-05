module;

#include <span>
#include <string>

export module args:remove;

import :command;

using namespace std;

namespace oul::args
{
	/// @brief Reprezentuje příkaz remove - odebrání souborů z komponenty.
	export class REMOVE: public COMMAND
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
		/// @brief Spustí příkaz remove programu OUL.
		void run() const override
		{
			COMPONENT_MANAGER manager = open_manager();
			span<const string> files(arguments.begin(), arguments.end());
			manager.remove_files(arguments[0], arguments[1], files);
		}
	};
}
