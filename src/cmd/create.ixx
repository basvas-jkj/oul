module;

#include <optional>
#include <string>

export module argument_parser:create;

import component_manager;
import :command;

namespace oul::args
{
	/// @brief Reprezentuje příkaz create - vytvoření nové komponenty.
	export class CREATE: public COMMAND
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
			else if (arguments.size() > 1)
			{
				report_error(ERROR::too_much_arguments);
				return false;
			}
			else
			{
				return true;
			}
		}
		/// @brief Spustí příkaz create programu OUL.
		void run() const override
		{
			cr<string> name = arguments[0];
			string location = opt.get("-w");
			if (location == "")
			{
				location = name;
			}

			optional<CONFIG> c = read_configuration();

			if (!c.has_value())
			{
				return;
			}
			else if (c->contains(name))
			{
				report_error(ERROR::component_already_exists);
				return;
			}

			COMPONENT_MANAGER manager(std::move(*c));
			manager.create(name, location);
		}
	};
}
