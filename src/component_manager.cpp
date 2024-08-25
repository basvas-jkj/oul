#include <ranges>
#include <iostream>
#include <algorithm>
#include <boost/filesystem.hpp>

#include "zip.hpp"
#include "server.hpp"
#include "general.hpp"
#include "component_manager.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace oul
{
	COMPONENT_MANAGER::COMPONENT_MANAGER(CONFIG&& c): c(c) 
	{};
	optional<COMPONENT_MANAGER> COMPONENT_MANAGER::init()
	{
		string conf = CONFIG::find();
		auto [valid, c] = CONFIG::read(conf);
		if (conf == "")
		{
			cerr << "Configuration file not found." << endl;
			cerr << "Call oul init or move into an initialized project." << endl;
			return {false, COMPONENT_MANAGER(std::move(c))};
		}
		else if (!valid)
		{
			cerr << "Configuration file is corrupted." << endl;
			cerr << "Remove it and initialize again or move into another project." << endl;
			return {false, COMPONENT_MANAGER(std::move(c))};
		}
		else
		{
			return {true, COMPONENT_MANAGER(std::move(c))};
		}
	}

	void COMPONENT_MANAGER::add_files(const string& name, const string& source_files, const string& test_files, const string& doc_files)
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
	void COMPONENT_MANAGER::remove_files(const string& name, const string& source_files, const string& test_files, const string& doc_files)
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

	void COMPONENT_MANAGER::add(const string& name, const string& save_as, const string& url, const string& where)
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
			ZIP_MANAGER zip(component);
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
	void COMPONENT_MANAGER::create(const string& name, const string& where, const string& source_files, const string& test_files, const string& doc_files)
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
	void COMPONENT_MANAGER::rename(const string& old_name, const string& new_name)
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
	void COMPONENT_MANAGER::move(const string& name, const string& new_location)
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
	void COMPONENT_MANAGER::remove(const string& name)
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