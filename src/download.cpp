#include <string>
#include <fstream>
#include <iostream>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "download.hpp"

using namespace std;
using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
	string download(const string& url)
	{
		path temp = unique_path("%%%%-%%%%-%%%%-%%%%.zip");
		path curl = search_path("curl");
		child ch(curl, url, "-o", temp, std_err > stderr);
		ch.wait();
		
		if (ch.exit_code() == 0)
		{
			return temp.string();
		}
	}
}
