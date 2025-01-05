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

			println("List of components used in this project:");
			for (cr<ITEM> i : c->components)
			{
				cr<string> url = (i.url.empty()) ? "local only" : i.url;
				println("\t{}\t({})", i.name, url);
			}

			return 0;
		}
	};
}