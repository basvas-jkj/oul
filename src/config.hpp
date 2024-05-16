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
		std::string default_url;
		std::vector<ITEM> components;
	};

	std::string find_configaration();
	std::optional<CONFIG> read_configuration(const std::string&);
	void initialize();

	void add_component(const std::string&, const std::string&, const std::vector<std::string>&);
}
