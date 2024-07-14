#pragma once

#include <string>

#include "config.hpp"

namespace oul
{
	void add_component(CONFIG& c, const std::string& name, const std::string& arg);
}