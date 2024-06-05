#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <libzippp/libzippp.h>

#include "config.hpp"

namespace oul
{
	class ZIP_COMPONENT
	{
		CONFIG c;
		std::string zip_file;
		libzippp::ZipArchive archiv;

	public:
		ZIP_COMPONENT(const std::string& zip);
		~ZIP_COMPONENT();

		std::vector<std::string> unzip(const std::string&, const std::string&);
	};
}