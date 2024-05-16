#include <fstream>
#include <iostream>
#include <filesystem>
#include <libzippp/libzippp.h>

#include "zip.hpp"

using namespace std;
using namespace std::filesystem;
using namespace libzippp;

namespace oul
{
	vector<string> get_content(const ZipArchive& component)
	{
		vector<string> content;
		for (ZipEntry& entry : component.getEntries())
		{
			content.push_back(entry.getName());
		}
		return content;
	}
	vector<string> unzip(const string& zip_component, const string& location)
	{
		ZipArchive component(zip_component);
		component.open();

		vector<string> content = get_content(component);
		for (string& entry_name : content)
		{
			path p = path(location) / path(entry_name);
			if (p.has_parent_path())
			{
				create_directories(p.parent_path());
			}

			ofstream output(p);
			ZipEntry entry = component.getEntry(entry_name);
			entry.readContent(output);
		}

		component.close();
		return content;
	}
}