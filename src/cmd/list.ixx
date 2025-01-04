export module args_list;

import <print>;

import args_command;

using namespace std;

export namespace oul::args
{
	class LIST: public COMMAND
	{
	public:
		int run() const override
		{
			optional<CONFIG> c = read_configuration();
			if (c == nullopt)
			{
				return 2;
			}

			println("Components included in this project:");
			for (cr<string> name : c->get_components())
			{
				println("\t{}", name);
			}

			return 0;
		}
	};
}