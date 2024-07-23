#include <iostream>
#include <filesystem>

#include "zip.hpp"
#include "general.hpp"
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
	void create_component(CONFIG& c, const string& name, const string& source_files, const string& test_files, const string& doc_files)
	{
		vector<string> source = split(source_files, ',');
		vector<string> tests = split(test_files, ',');
		vector<string> docs = split(doc_files, ',');

		ITEM i{name, "", docs, source, tests};

		c.add_component(i);
	}
}