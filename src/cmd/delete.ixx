export module argument_parser:_delete;

import component_manager;
import :command;

namespace oul::args
{
	/// @brief Reprezentuje příkaz delete - odebrání komponenty z projektu.
	export class DELETE: public COMMAND
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
				return false;
			}
			else if (arguments.size() > 1)
			{
				report_error(MESSAGE::too_much_arguments);
				return false;
			}
			else
			{
				return true;
			}
		}
		/// @brief Spustí příkaz delete programu OUL.
		void run() const override
		{
			COMPONENT_MANAGER manager = open_manager();
			manager.remove(arguments[0]);
		}
	};
}
