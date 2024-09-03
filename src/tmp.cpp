#include "tmp.hpp"

using namespace std;
using namespace boost::filesystem;

namespace oul
{
	path TMP_FILE::get_temporary_folder(bool use_subfolder)
	{
		path temp = temp_directory_path() / "oul";

		if (use_subfolder)
		{
			temp /= unique_path();
		}

		create_directory(temp);
		return temp;
	}
	
	TMP_FILE::TMP_FILE(const string& name, bool use_subfolder): name(unique_path(get_temporary_folder(use_subfolder) / name)) {}
	TMP_FILE::TMP_FILE(const path& name): name(name) {}
	TMP_FILE::~TMP_FILE()
	{
		remove(name);
	}

	path TMP_FILE::get_name() const
	{
		return name;
	}
	ofstream TMP_FILE::write() const
	{
		return ofstream(name.c_str());
	}
	ifstream TMP_FILE::read() const
	{
		return ifstream(name.c_str());
	}
}
