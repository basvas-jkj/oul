export module args:group_remove;

import :command;

using namespace std;

export namespace oul::args
{
	class GROUP_REMOVE: public COMMAND
	{
	public:
		bool check() const override
		{
			if (arguments.size() < 1)
			{
				report_error(message::missing_component_name);
			}
			if (arguments.size() < 2)
			{
				report_error(message::missing_group_name);
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
			manager.remove_group(arguments[0], arguments[1]);
		}
	};
}