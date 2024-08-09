#include "command.hpp"

using namespace std;

int COMMAND::next_index = 1;
vector<COMMAND> COMMAND::list;
map<string, COMMAND&> COMMAND::dictionary;

void COMMAND::init()
{
	list =
	{
		{"init", "     creates configuration file"},
		{"add", "      downloads component and adds it into configuration"},
		{"create", "   creates new component locally"},
		{"list", "     writes list of all installed or localy created components"},
		{"rename", "   changes the name of the component"}
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