export module args_create;

import message;
import args_command;
import component_manager;

using namespace std;

export namespace oul::args
{
	class CREATE: public COMMAND
	{
	public:
		int run() const override
		{
			if (arguments.size() < 1)
			{
				report_error(message::missing_component_name);
				return 3;
			}
			cr<string> name = arguments[0];
			string location = opt.get("-w");
			if (location == "")
			{
				location = name;
			}

			optional<CONFIG> c = read_configuration();
			
			if (!c.has_value())
			{
				return 2;
			}
			else if (c->contains(name))
			{
				report_error(message::component_already_exists);
				return 1;
			}
			
			COMPONENT_MANAGER manager(move(*c));
			manager.create(name, location);
			return 0;
		}
	};
}