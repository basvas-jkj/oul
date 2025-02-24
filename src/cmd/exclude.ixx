export module args:exclude;

import component_manager;
import message;
import :command;

using namespace std;

export namespace oul::args
{
	class EXCLUDE: public COMMAND
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
			}
			if (arguments.size() < 3)
			{
				report_error(message::empty_file_list);
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
			span<const string> files(arguments.begin() + 2, arguments.end());
			manager.exclude_files(arguments[0], arguments[1], files);
		}
	};
}