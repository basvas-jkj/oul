export module args;

import <map>;
import <memory>;
import <string>;
import <vector>;
import <optional>;
using namespace std;

import usings;

const map<string, string> short_variants = {
	{"-version", "-v"},
	{"-where", "-w"}
};

export namespace oul
{
	class OPTIONS
	{
		map<string, string> options;

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
}