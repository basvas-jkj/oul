#include <string>
#include <fstream>
#include <iostream>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "server.hpp"
#include "support.hpp"

using namespace std;
using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
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
	string download(const string& url)
	{
		path temp = unique_path(get_temporary_folder() / "%%%%-%%%%-%%%%-%%%%.zip");
		path curl = search_path("curl");
		child ch(curl, url, "-o", temp, std_err > null);
		ch.wait();
		
		if (ch.exit_code() == 0)
		{
			return temp.string();
		}
		else
		{
			cerr << "Component could not be downloaded." << endl;
			return "";
		}
	}
}
