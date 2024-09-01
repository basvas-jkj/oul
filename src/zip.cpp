#include <iostream>

#include "zip.hpp"
#include "support.hpp"
#include "config/data_file.hpp"
#include "config/data_file.hpp"
#include "../lib/helpers/Helpers.h"

using namespace std;
using namespace Zip;

namespace oul
{
	static void move(const fs::path& source_directory, const string& target_directory, const string& entry_name)
	{
		fs::path source = fs::path(source_directory) / fs::path(entry_name);
		fs::path target = fs::path(target_directory) / fs::path(entry_name);
		if (target.has_parent_path())
		{
			create_directories(target.parent_path());
		}

		fs::rename(source, target);
	}

	ZIP_MANAGER::ZIP_MANAGER(const string& zip): zip_file(zip), extracted_directory(fs::path(zip_file).replace_extension())
	{
		bool success;
		fs::create_directory(extracted_directory);
		string config = ExtractTextFromZip(zip_file, "oul.component.json", success);
		
		if (!success)
		{
			config = ExtractTextFromZip(zip_file, "oul.component.yaml", success);
		}

		c = read_component(config).second;
	}
	ZIP_MANAGER::~ZIP_MANAGER()
	{
		fs::remove(zip_file);
		fs::remove_all(extracted_directory);
	}
	ITEM ZIP_MANAGER::unzip(const string& where)
	{
		size_t count;
		
		if (ExtractAllFilesFromZip(extracted_directory.string() + "/", zip_file, count))
		{
			vector content{c.source_files, c.test_files, c.documentation};
			for (string& entry_name : content | views::join)
			{
				move(extracted_directory, where, entry_name);
			}
		}

		return c;
	}
}