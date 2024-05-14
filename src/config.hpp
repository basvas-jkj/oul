#pragma once

#include <string>
#include <vector>
#include <optional>

namespace oul
{
	struct ITEM
	{
		std::string url;
		std::vector<std::string> content;
	};
	struct CONFIG
	{
		std::string name;
		std::vector<ITEM> components;
	};

	std::string find_configaration();
	std::optional<CONFIG> read_configuration(const std::string&);
	void initialize();
}
