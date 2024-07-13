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
		ITEM c;
		std::string zip_file;
		libzippp::ZipArchive archiv;

	public:
		ZIP_COMPONENT(const std::string& zip);
		~ZIP_COMPONENT();

		ITEM unzip(const std::string&);
	};
}