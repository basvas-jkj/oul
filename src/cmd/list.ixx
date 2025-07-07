module;

#include <string>

export module argument_parser:list;

import :command;
import message;

namespace oul::args
{
	/// @brief Reprezentuje příkaz list - vypsání seznamu komponent v projektu.
	export class LIST: public COMMAND
	{
	public:
		/// @brief Spustí příkaz list programu OUL.
		void run() const override
		{
			cr<string> url = opt.get("-url");
			if (url == "")
			{
				CONFIG c = read_configuration();
				print_component_list(c.components);
			}
			else
			{
				COMPONENT_MANAGER::list_server_components(url);
			}
		}
	};
}
