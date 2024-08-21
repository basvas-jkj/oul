#include <ranges>
#include <iostream>
#include <algorithm>
#include <filesystem>

#include "zip.hpp"
#include "server.hpp"
#include "general.hpp"
#include "component_manager.hpp"

using namespace std;
namespace fs = std::filesystem;

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
	void remove_files(CONFIG& c, const string& name, const string& source_files, const string& test_files, const string& doc_files)
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

		for (string& entry : source)
		{
			vector<string>::iterator it = ranges::find(i->source_files, entry);
			i->source_files.erase(it);
		}
		for (string& entry : tests)
		{
			vector<string>::iterator it = ranges::find(i->test_files, entry);
			i->test_files.erase(it);
		}
		for (string& entry : source)
		{
			vector<string>::iterator it = ranges::find(i->documentation, entry);
			i->documentation.erase(it);
		}
	}

	void add_component(CONFIG& c, const string& name, const string& save_as, const string& url, const string& where)
	{
		if (c.contains(name))
		{
			cerr << "This project already contains a component of the same name." << endl;
			return;
		}
		
		string component_url;
		if (url == "")
		{
			component_url = c.get_url(name);
		}
		else
		{
			component_url = url + "/" + name;
		}

		string component = download(component_url);

		if (component != "")
		{
			ZIP_COMPONENT zip(component);
			ITEM i(zip.unzip(where));
			i.url = url;
			i.original_name = name;

			if (save_as != "")
			{
				i.name = save_as;
			}
			if (where != "")
			{
				i.location = where;
			}


			c.add_component(i);
		}
		else
		{
			cerr << "This component can't be downloaded." << endl;
		}
	}
	void create_component(CONFIG& c, const string& name, const string& where, const string& source_files, const string& test_files, const string& doc_files)
	{
		if (c.contains(name))
		{
			cerr << "This project already contains a component of the same name." << endl;
			return;
		}

		vector<string> source = split(source_files, ',');
		vector<string> tests = split(test_files, ',');
		vector<string> docs = split(doc_files, ',');

		ITEM i{name, where, "", "", docs, source, tests};

		c.add_component(i);
	}
	void rename_component(CONFIG& c, const string& old_name, const string& new_name)
	{
		vector<ITEM>::iterator i = c.get_component(old_name);
		if (i == c.components.end())
		{
			cerr << "There is no component of this name." << endl;
		}
		else
		{
			i->name = new_name;
		}
	}
	void move_component(CONFIG& c, const string& name, const string& new_location)
	{
		vector<ITEM>::iterator i = c.get_component(name);
		if (i == c.components.end())
		{
			cerr << "There is no component of this name." << endl;
		}
		else
		{
			vector content{i->source_files, i->test_files, i->documentation};
			for (fs::path entry : content | views::join)
			{
				fs::rename(i->location / entry, new_location / entry);
			}

			i->location = new_location;
		}
	}
	void remove_component(CONFIG& c, const string& name)
	{
		vector<ITEM>::iterator i = c.get_component(name);
		if (i == c.components.end())
		{
			cerr << "There is no component of this name." << endl;
		}
		else
		{
			vector content{i->source_files, i->test_files, i->documentation};
			for (fs::path entry : content | views::join)
			{
				fs::remove_all(i->location / entry);
			}
		}
	}
}