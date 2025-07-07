module;

#include <string>

export module argument_parser:rename;

import :command;
import message;

using namespace std;

namespace oul::args
{
	/// @brief Reprezentuje příkaz rename - přejmenování komponenty.
	export class RENAME: public COMMAND
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
				return false;
			}
			if (arguments.size() < 2)
			{
				report_error(ERROR::missing_component_name);
				return false;
			}
			else if (arguments.size() > 2)
			{
				report_error(ERROR::too_much_arguments);
				return false;
			}
			else
			{
				return true;
			}
		}
		/// @brief Spustí příkaz rename programu OUL.
		void run() const override
		{
			cr<string> original_name = arguments[0];
			cr<string> new_name = arguments[1];

			COMPONENT_MANAGER manager = open_manager();
			manager.rename(original_name, new_name);
		}
	};
}
