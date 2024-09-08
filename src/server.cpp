#include <string>
#include <fstream>
#include <iostream>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include "../lib/helpers/Helpers.h"

#include "tmp.hpp"
#include "server.hpp"
#include "support.hpp"
#include "config/data_file.hpp"

using namespace std;
using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
	static optional<ITEM> unzip(const TMP_FILE& zip_file, const string& where)
	{
		bool success;
		ITEM component;
		string zip_name = zip_file.get_name().string();

		string config = Zip::ExtractTextFromZip(zip_name, "oul.component.json", success);
		if (!success)
		{
			config = Zip::ExtractTextFromZip(zip_name, "oul.component.yaml", success);
			if (!success)
			{
				return {false, component};
			}
		}

		component = read_component(config).second;

		size_t count;
		TMP_FOLDER extracted_zip(zip_file.get_name().replace_extension());
		if (Zip::ExtractAllFilesFromZip(extracted_zip.get_name().string(), zip_name, count))
		{
			if (count < component.source_files.size() + component.test_files.size() + component.documentation.size())
			{
				return {false, component};
			}

			vector content{component.source_files, component.test_files, component.documentation};
			for (string& entry_name : content | views::join)
			{
				move(extracted_zip.get_name(), where, entry_name);
			}
		}

		return {true, component};
	}

	void registration(const CONFIG& c)
	{
		string admin_name;
		cout << "Admin username: ";
		cin >> admin_name;

		string admin_password;
		cout << "Admin password: ";
		cin >> admin_password;

		string new_user_name;
		cout << "New username: ";
		cin >> new_user_name;

		string new_user_password;
		cout << "New password: ";
		cin >> new_user_password;
		cin.get();

		cout << "Is new user administrator (n)? ";
		bool new_admin = cin.get() == 'y';

		string admin_auth = format("{}:{}", admin_name, admin_password);
		string new_auth = format("{}:{}", new_user_name, new_user_password);
		string url = c.get_url("register");
		if (new_admin)
		{
			url += "?admin=true";
		}
		path curl = search_path("curl");
		child ch(curl, "--fail-with-body", "-X", "PUT", "-d", new_auth, "-u", admin_auth, url, std_out > stdout, std_err > null);
		ch.wait();

		if (ch.exit_code() == 0)
		{
			cerr << "Registration was successful." << endl;
		}
		else
		{
			cerr << endl << "Registration was not successful." << endl;
		}
	}
	optional<ITEM> download(const string& url, const string& where)
	{
		TMP_FILE file("%%%%-%%%%-%%%%-%%%%.zip", false);
		path curl = search_path("curl");
		child ch(curl, url, "-o", file.get_name(), std_err > null);
		ch.wait();
		
		if (ch.exit_code() == 0)
		{
			return unzip(file, where);
		}
		else
		{
			cerr << "Component could not be downloaded." << endl;
			return {false, ITEM()};
		}
	}


}
