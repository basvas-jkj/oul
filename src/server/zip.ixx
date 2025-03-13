module;

#include <boost/process.hpp>
#undef ERROR

export module zip;
import :base;
import :tools;

namespace oul
{
	export using zip_tool_ptr = unique_ptr<ABSTRACT_ZIP_TOOL>;
	export zip_tool_ptr select_zip_tool()
	{
		path tool_path = search_path("zip");
		if (tool_path != "")
		{
			//return make_unique<TOOL_ZIP>(tool_path);
		}
		tool_path = search_path("7z");
		if (tool_path != "")
		{
			return make_unique<TOOL_7Z>(tool_path);
		}
		throw MissingZipTool(missing_zip_tool);
	}
}