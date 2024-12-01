export module command;

import <map>;
import <memory>;
import <string>;
import <vector>;

import usings;

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
		OPTIONS opt;
		vector<string> arguments;

	public:
		void init(OPTIONS&& opt, vector<string>&& arguments)
		{
			this->opt = move(opt);
			this->arguments = move(arguments);
		}
		virtual void run() const = 0;
	};
}