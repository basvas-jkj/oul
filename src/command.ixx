export module args:command;

import std;
import usings;
import common;
import message;
import component_manager;

using namespace std;

namespace oul
{
	const map<string, string> short_variants = {
		{"-version", "-v"},
		{"-where", "-w"}
	};
	export class ConfigurationNotFound: public CommonException
	{
	public:
		ConfigurationNotFound(ERROR name): CommonException(name)
		{}
	};
	export class OPTIONS
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
				static string empty = "";
				return empty;
			}
			else
			{
				return it->second;
			}
		}
	};
	export class COMMAND
	{
	protected:
		OPTIONS opt;
		vector<string> arguments;

		static string find_configuration()
		{
			optional<string> path = CONFIG::find();
			if (path == nullopt)
			{
				throw ConfigurationNotFound(config_not_found);
			}
			else
			{
				return *path;
			}
		}
		static CONFIG read_configuration()
		{
			string path = find_configuration();
			return CONFIG(path);
		}
		static COMPONENT_MANAGER open_manager()
		{
			CONFIG c = read_configuration();
			COMPONENT_MANAGER manager(move(c));
			return manager;
		}

	public:
		virtual ~COMMAND() = default;

		void init(OPTIONS&& opt, vector<string>&& arguments)
		{
			this->opt = move(opt);
			this->arguments = move(arguments);
		}
		virtual bool check() const
		{
			return true;
		}
		virtual void run() const = 0;
	};
}