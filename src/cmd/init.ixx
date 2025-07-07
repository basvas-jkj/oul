module;

#include <optional>
#include <string>

export module argument_parser:init;

import configuration;
import :command;

using namespace std;

namespace oul::args
{
	/// @brief Reprezentuje příkaz init - vytvoření konfigurace projektu.
	export class INIT: public COMMAND
	{
	public:
		/// @brief Spustí příkaz init programu OUL.
		void run() const override
		{
			optional<string> path = CONFIG::find();
			if (path.has_value())
			{
				report_error(ERROR::config_found);
			}
			else
			{
				CONFIG::initialize();
			}
		}
	};
}
