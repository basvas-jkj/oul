module;

#include <boost/filesystem/operations.hpp>
#include <string>

export module argument_parser:upload;

import :command;
import message;
import support;

using namespace std;

namespace fs = boost::filesystem;
namespace oul::args
{
	/// @brief Reprezentuje příkaz upload - nahrání komponenty na server.
	export class UPLOAD: public COMMAND
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
		/// @brief Spustí příkaz upload programu OUL.
		void run() const override
		{
			cr<string> local_name = arguments[0];
			CONFIG cfg = read_configuration();
			vector<ITEM>::iterator i = cfg.get_component(local_name);
			if (i == cfg.components.end())
			{
				throw CommonException(MESSAGE::component_not_found);
			}
			ITEM& component = *i;

			string server_name = opt.get("-as");
			if (server_name == "")
			{
				server_name = local_name;
				component.original_name = local_name;
			}

			string url = opt.get("-url");
			if (url == "")
			{
				if (component.url != "")
				{
					url = component.url;
				}
				else if (cfg.default_url != "")
				{
					url = cfg.default_url;
				}
				else
				{
					throw CommonException(MESSAGE::missing_url);
				}
			}
			else if (component.url == "")
			{
				component.url = url;
			}

			fs::path location = cfg.get_directory() / component.location;

			COMPONENT_MANAGER manager(std::move(cfg));
			manager.upload(server_name, url, location, component);
		}
	};
}
