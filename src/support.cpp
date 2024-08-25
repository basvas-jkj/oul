#include "support.hpp"

using namespace std;

namespace oul
{
	void move(const fs::path& source_directory, const string& target_directory, const string& entry_name)
	{
		path source = path(source_directory) / path(entry_name);
		path target = path(target_directory) / path(entry_name);
		if (target.has_parent_path())
		{
			create_directories(target.parent_path());
		}

		rename(source, target);
	}
}