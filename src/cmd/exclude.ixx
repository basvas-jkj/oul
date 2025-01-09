export module args_exclude;

import args_command;
import component_manager;
import message;

using namespace std;

export namespace oul::args
{
	class EXCLUDE: public COMMAND
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

			span<const string> files(arguments.begin() + 2, arguments.end());
			manager.exclude_files(arguments[0], arguments[1], files);

			return 0;
		}
	};
}