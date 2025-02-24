export module args:init;

import std;
import config;
import message;
import :command;

using namespace std;

export namespace oul::args
{
	class INIT: public COMMAND
	{
	public:
		void run() const override
		{
			optional<string> path = CONFIG::find();
			if (path.has_value())
			{
				report_error(message::config_found);
			}
			else
			{
				CONFIG::initialize();
			}
		}
	};
}