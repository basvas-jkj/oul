#pragma once

#include <map>
#include <span>
#include <vector>
#include <string>

class COMMAND
{
	static int next_index;
	static std::vector<COMMAND> list;
	static std::map<std::string, COMMAND&> dictionary;

	int index;
	std::string name;
	std::string short_help;
	COMMAND(const std::string& name, const std::string& short_help): name(name), short_help(short_help)
	{
		index = next_index;
		next_index += 1;
	}

public:
	static void init();
	static COMMAND& none();
	static COMMAND& find(const std::string& name);
	static std::span<COMMAND> all();
	std::string get_name() const;
	std::string get_short_help() const;
	bool operator==(const COMMAND& o) const;
	bool is(const std::string& name) const;
};