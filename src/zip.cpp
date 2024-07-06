#include <iostream>
#include <filesystem>

#include "zip.hpp"

using namespace std;
using namespace libzippp;
namespace fs = std::filesystem;

namespace oul
{
	ZIP_COMPONENT::ZIP_COMPONENT(const string& zip): zip_file(zip), archiv(zip)
	{
		archiv.open();

		if (archiv.hasEntry("oul.config.json"))
		{
			using namespace nlohmann;
			json config(archiv.getEntry("oul.config.json").readAsText());
			c = CONFIG::read_json(config).second;
			
		}
		else if (archiv.hasEntry("oul.config.yaml"))
		{
			using namespace YAML;
			Node config(archiv.getEntry("oul.config.json").readAsText());
			c = CONFIG::read_yaml(config).second;
		}
	}
	ZIP_COMPONENT::~ZIP_COMPONENT()
	{
		archiv.close();
		fs::remove(zip_file);
	}
	vector<string> ZIP_COMPONENT::unzip(const string& zip_component, const string& location)
	{
		auto view = c.get_components();
		vector<string> content(view.begin(), view.end());

		for (const std::string& entry_name : content)
		{
			fs::path p = fs::path(location) / fs::path(entry_name);
			if (p.has_parent_path())
			{
				create_directories(p.parent_path());
			}

			ofstream output(p);
			ZipEntry entry = archiv.getEntry(entry_name);
			entry.readContent(output);
		}

		return content;
	}
}