export module args:_delete;

import config;
import component_manager;
import :command;

using namespace std;

export namespace oul::args
{
	class DELETE: public COMMAND
	{
	public:
		bool check() const override
		{
			if (arguments.size() < 1)
			{
				report_error(message::missing_component_name);
				return false;
			}
			else
			{
				return true;
			}
		}
		void run() const override
		{
			COMPONENT_MANAGER manager = open_manager();
			manager.remove(arguments[0]);
		}
	};
}