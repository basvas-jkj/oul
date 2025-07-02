export module args:group_add;

import :command;

namespace oul::args
{
	/// @brief Reprezentuje příkaz group add - vytvoření skupiny souborů.
	export class GROUP_ADD: public COMMAND
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
		/// @brief Spustí příkaz group add programu OUL.
		void run() const override
		{
			COMPONENT_MANAGER manager = open_manager();
			manager.add_group(arguments[0], arguments[1]);
		}
	};
}
