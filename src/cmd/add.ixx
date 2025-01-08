export module args_add;

import message;
import args_command;
import component_manager;

using namespace std;

export namespace oul::args
{
	class ADD: public COMMAND
	{
	public:
		int run() const override
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
				return 3;
			}
			
			optional<CONFIG> c = read_configuration();
			if (c == nullopt)
			{
				return 2;
			}

			COMPONENT_MANAGER manager(move(*c));
			
			span<const string> files(arguments.begin(), arguments.end());
			manager.add_files(arguments[0], arguments[1], files);

			return 0;
		}
	};
}