#include "support.hpp"

using namespace std;

namespace oul
{
	void move(const fs::path& source_directory, const string& target_directory, const string& entry_name)
	{
		fs::path source = fs::path(source_directory) / fs::path(entry_name);
		fs::path target = fs::path(target_directory) / fs::path(entry_name);
		if (target.has_parent_path())
		{
			fs::create_directories(target.parent_path());
		}

		rename(source, target);
	}
}
