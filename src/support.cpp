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
	fs::path get_temporary_folder(bool use_subfolder)
	{
		fs::path temp = fs::temp_directory_path() / "oul";

		if (use_subfolder)
		{
			temp /= fs::unique_path();
		}

		fs::create_directory(temp);
		return temp;
	}
}
