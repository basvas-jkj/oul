export module args_init;

import <iostream>;

import args_command;
import config;
import message;

using namespace std;

export namespace oul::args
{
	class INIT: public COMMAND
	{
	public:
		int run() const override
		{
			optional<string> path = CONFIG::find();
			if (path.has_value())
			{
				report_error(message::config_found);
				return 2;
			}
			else
			{
				CONFIG::initialize();
				return 0;
			}
		}
	};
}