#pragma once

#include <tuple>
#include <string>

#include "../config.hpp"

namespace oul
{
	template <class T>
	using optional = std::pair<bool, T>;

	optional<ITEM> read_component(const std::string&);
	optional<CONFIG> load_config(const std::string&);

	void write_config(const CONFIG&);
}