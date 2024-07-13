#include <iostream>
#include <filesystem>

#include "zip.hpp"
#include "data_file/data_file.hpp"

using namespace std;
using namespace libzippp;
namespace fs = std::filesystem;

namespace oul
{
	ZIP_COMPONENT::ZIP_COMPONENT(const string& zip): zip_file(zip), archiv(zip)
	{
		archiv.open();

		if (archiv.hasEntry("oul.component.json"))
		{
			c = read_component(archiv.getEntry("oul.component.json").readAsText()).second;
		}
		else if (archiv.hasEntry("oul.component.yaml"))
		{
			c = read_component(archiv.getEntry("oul.component.yaml").readAsText()).second;
		}
	}
	ZIP_COMPONENT::~ZIP_COMPONENT()
	{
		archiv.close();
		fs::remove(zip_file);
	}
	ITEM ZIP_COMPONENT::unzip(const string& where)
	{
		for (string& entry_name : c.source_files)
		{
			fs::path p = fs::path(where) / fs::path(entry_name);
			if (p.has_parent_path())
			{
				create_directories(p.parent_path());
			}

			ofstream output(p);
			ZipEntry entry = archiv.getEntry(entry_name);
			entry.readContent(output);
		}

		return c;
	}
}