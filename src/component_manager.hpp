#pragma once

#include <string>

#include "config.hpp"

namespace oul
{
	void add_component(CONFIG& c, const std::string& name, const std::string& arg);
	void create_component(CONFIG& c, const std::string& name, const std::string& source_files, const std::string& test_files, const std::string& doc_files);
	void rename_component(CONFIG& c, const std::string& old_name, const std::string& new_name);
}