#pragma once

#include <map>
#include <span>
#include <deque>
#include <vector>
#include <string>

namespace oul
{
	void write_short_help();

	class COMMAND
	{
		static int next_index;
		static std::vector<COMMAND> list;
		static std::map<std::string, COMMAND&> dictionary;

		int index;
		std::string name;
		std::string short_help;
		COMMAND(const std::string& name, const std::string& short_help);

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
	class ARGS
	{
		COMMAND* c;
		std::map<std::string, std::string> options;
		std::deque<std::string> arguments;

	public:
		ARGS(int argc, char* argv[]);
		std::string next_arg();
		bool is(const std::string& name) const;
		bool has_options(const std::string& option) const;
		std::string get_option(const std::string& option);
	};
}