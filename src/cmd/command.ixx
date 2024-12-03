export module args_command;

import <map>;
import <memory>;
import <optional>;
import <string>;
import <vector>;

import usings;
import message;

using namespace std;
using namespace oul;

const map<string, string> short_variants = {
	{"-version", "-v"},
	{"-where", "-w"}
};

export namespace oul
{
	class OPTIONS
	{
		map<string, string> options;

	public:
		void add(cr<string> name, cr<string> value = "")
		{
			auto it = short_variants.find(name);

			if (it == short_variants.end())
			{
				options[name] = value;
			}
			else
			{
				cr<string> short_name = it->second;
				options[short_name] = value;
			}
		}
		bool includes(cr<string> name) const
		{
			return options.find(name) != short_variants.end();
		}
		cr<string> get(cr<string> name) const
		{
			auto it = options.find(name);
			if (it == options.end())
			{
				return "";
			}
			else
			{
				return it->second;
			}
		}
	};
	class COMMAND
	{
	protected:
		OPTIONS opt;
		vector<string> arguments;

		optional<string> find_configuration()
		{
			optional<string> path = CONFIG::find();
			if (path == nullopt)
			{
				report_error(message::config_not_found);
				return nullopt;
			}
		}
		optional<CONFIG> read_configuration()
		{
			optional<string> path = find_configuration();
			if (path == nullopt)
			{
				return nullopt;
			}
			else
			{
				return CONFIG::read(*path);
			}
		}

	public:
		void init(OPTIONS&& opt, vector<string>&& arguments)
		{
			this->opt = move(opt);
			this->arguments = move(arguments);
		}
		virtual int run() const = 0;
	};
}