#include <iostream>
#include <algorithm>
#include <filesystem>

#include "zip.hpp"
#include "general.hpp"
#include "download.hpp"
#include "component_manager.hpp"

using namespace std;

namespace oul
{
	void add_files(CONFIG& c, const string& name, const string& source_files, const string& test_files, const string& doc_files)
	{
		vector<string> source = split(source_files, ',');
		vector<string> tests = split(test_files, ',');
		vector<string> docs = split(doc_files, ',');

		vector<ITEM>::iterator i = c.get_component(name);

		if (i == c.components.end())
		{
			cerr << "There is no component of this name." << endl;
			return;
		}

		i->source_files.insert(i->source_files.end(), source.begin(), source.end());
		i->test_files.insert(i->test_files.end(), tests.begin(), tests.end());
		i->documentation.insert(i->documentation.end(), docs.begin(), docs.end());
	}
	void add_component(CONFIG& c, const string& name, const string& save_as)
	{
		if (c.contains(name))
		{
			cerr << "This project already contains a component of the same name." << endl;
			return;
		}
		
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
		if (c.contains(name))
		{
			cerr << "This project already contains a component of the same name." << endl;
			return;
		}

		vector<string> source = split(source_files, ',');
		vector<string> tests = split(test_files, ',');
		vector<string> docs = split(doc_files, ',');

		ITEM i{name, "", docs, source, tests};

		c.add_component(i);
	}
	void rename_component(CONFIG& c, const string& old_name, const string& new_name)
	{
		auto predicate = [old_name](const ITEM& i)
			{
				return i.name == old_name;
			};
		auto i = ranges::find_if(c.components, predicate);
		i->name = new_name;
	}
}