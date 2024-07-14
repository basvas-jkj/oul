#include <iostream>
#include <filesystem>

#include "zip.hpp"
#include "download.hpp"
#include "component_manager.hpp"

using namespace std;

namespace oul
{
	void add_component(CONFIG& c, const string& name, const string& arg)
	{
		string url = c.get_url(name);
		string component = download(url);

		if (component != "")
		{
			ZIP_COMPONENT zip(component);
			ITEM i(zip.unzip(""));

			c.add_component(i);
		}
		else
		{
			cerr << "This component can't be downloaded." << endl;
		}
	}
}