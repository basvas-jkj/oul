#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace oul
{
    void move(const fs::path& source_directory, const std::string& target_directory, const std::string& entry_name);
    fs::path get_temporary_folder();
}