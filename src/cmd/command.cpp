#include "args.hpp"

using namespace std;

namespace oul
{
	int COMMAND::next_index = 1;
	vector<COMMAND> COMMAND::list;
	map<string, COMMAND&> COMMAND::dictionary;

	COMMAND::COMMAND(const string& name, const string& short_help): name(name), short_help(short_help)
	{
		index = next_index;
		next_index += 1;
	}

	void COMMAND::init()
	{
		list =
		{
			{"init", "     creates configuration file"},
			{"install", "  downloads component and adds it into configuration"},
			{"delete", "   deletes the component"},
			{"add", "      adds files to the component"},
			{"remove", "   removes files from the component"},
			{"create", "   creates new component locally"},
			{"list", "     writes list of all installed or localy created components"},
			{"rename", "   changes the name of the component"},
			{"move", "     changes the location of the component"},
			{"register", " creates an account on the server"}
		};
		list.reserve(list.size() + 1);

		for (COMMAND& c : list)
		{
			dictionary.insert({c.name, c});
		}
		list.push_back({"none", "---"});
	}
	COMMAND& COMMAND::none()
	{
		return list.back();
	}
	COMMAND& COMMAND::find(const string& name)
	{
		auto i = dictionary.find(name);

		if (i == dictionary.end())
		{
			return none();
		}
		else
		{
			return i->second;
		}
	}
	span<COMMAND> COMMAND::all()
	{
		return {list.begin(), list.end() - 1};
	}

	string COMMAND::get_name() const
	{
		return name;
	}
	string COMMAND::get_short_help() const
	{
		return short_help;
	}
	bool COMMAND::operator==(const COMMAND& o) const
	{
		return index == o.index;
	}
	bool COMMAND::is(const string& name) const
	{
		return this->name == name;
	}
}