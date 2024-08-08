#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>

#include "config.hpp"

namespace oul
{
	class ZIP_COMPONENT
	{
		ITEM c;
		std::string zip_file;
		boost::filesystem::path extracted_directory;

	public:
		ZIP_COMPONENT(const std::string& zip);
		~ZIP_COMPONENT();

		ITEM unzip(const std::string&);
	};
}