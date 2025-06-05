module;

#include <boost/filesystem/path.hpp>
#include <boost/process.hpp>
#undef ERROR

export module zip;
import :base;
import :tools;

using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
	export using zip_tool_ptr = unique_ptr<ABSTRACT_ZIP_TOOL>;

	/// @brief Najde libovolný nástroj pro vytváření a rozbalování zip archivů.
	/// Podporované nástroje: 7z, zip, unzip
	/// @return Objekt reprezentující nalezený nástroj.
	export zip_tool_ptr select_zip_tool()
	{
		path seven_zip_path = search_path("7z");
		if (seven_zip_path != "")
		{
			return make_unique<TOOL_7Z>(seven_zip_path);
		}
		path zip_path = search_path("zip");
		path unzip_path = search_path("unzip");
		if (zip_path != "" && unzip_path != "")
		{
			return make_unique<TOOL_ZIP>(zip_path, unzip_path);
		}
		throw MissingZipTool(ERROR::missing_zip_tool);
	}
}
