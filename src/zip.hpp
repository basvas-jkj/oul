#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>

#include "config.hpp"

namespace oul
{
	class ZIP_MANAGER
	{
		ITEM c;
		std::string zip_file;
		boost::filesystem::path extracted_directory;

	public:
		ZIP_MANAGER(const std::string& zip);
		~ZIP_MANAGER();

		ITEM unzip(const std::string&);
	};
}