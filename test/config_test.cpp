#include <fstream>
#include <filesystem>

#include "cppunit.h"
#include "../src/config.hpp"

namespace fs = std::filesystem;

using namespace std;
using namespace oul;

const fs::path test_files = "../../test/files";

bool compare_files(const fs::path& path_1, const fs::path& path_2)
{
	ifstream file_1(path_1);
	ifstream file_2(path_2);

	while (true)
	{
		if (file_1.get() != file_2.get())
		{
			return false;
		}
		if (file_1.fail() && file_2.fail())
		{
			return true;
		}
		else if (file_1.fail() || file_2.fail())
		{
			return false;
		}
	}
}
 
class CONFIG_TEST: public Cppunit
{
	void create_json()
	{
		cout << endl << "BEGIN: create_json()" << endl;
		test_cin("try Json http://127.0.0.1:8080");
		CONFIG::initialize();

		fs::path cfg = "oul.config.json";
		CHECKT(fs::exists(cfg));
		CHECKT(compare_files(cfg, test_files / "correct.json"));
		fs::remove(cfg);
		cout << endl << "END: create_json()" << endl;
	}
	void create_yaml()
	{
		cout << endl << "BEGIN: create_yaml()" << endl;
		test_cin("try Yaml http://127.0.0.1:8080");
		CONFIG::initialize();

		fs::path cfg = "oul.config.yaml";
		CHECKT(fs::exists(cfg));
		CHECKT(compare_files(cfg, test_files / "correct.yaml"));
		fs::remove(cfg);
		cout << endl << "END: create_yaml()" << endl;
	}
	void read_json()
	{
		cout << endl << "BEGIN: read_json()" << endl;
		fs::copy_file(test_files / "oul.config.json", "./oul.config.json");
		fs::path cfg = CONFIG::find();
		
		auto [valid, c] = CONFIG::read();
		CHECKT(valid);
		CHECKS(c.location, cfg.string());

		CHECKS(c.name, "try");
		CHECKS(c.default_url, "http://127.0.0.1:8080/");
		CHECK(c.components.size(), 2);

		CHECKS(c.components[0].name, "example");
		CHECKT(c.components[0].documentation == vector<string>{"doc"});
		CHECKT(c.components[0].source_files == vector<string>{"src"});
		CHECKT(c.components[0].test_files == vector<string>{"test"});
		CHECKS(c.components[0].url, "http://127.0.0.1:8080/");

		CHECKS(c.components[1].name, "experiment");
		CHECKT(c.components[1].documentation == vector<string>{"docs"});
		CHECKT(c.components[1].source_files == vector<string>{"source"});
		CHECKT(c.components[1].test_files == vector<string>{"tests"});
		CHECKS(c.components[1].url, "http://127.0.0.1:8080/");

		fs::remove(cfg);
		cout << endl << "END: read_json()" << endl;
	}
	void read_yaml()
	{
		cout << endl << "BEGIN: read_yaml()" << endl;
		fs::copy_file(test_files / "oul.config.yaml", "./oul.config.yaml");
		fs::path cfg = CONFIG::find();

		auto [valid, c] = CONFIG::read();
		CHECKT(valid);
		CHECKS(c.location, cfg.string());

		CHECKS(c.name, "try");
		CHECKS(c.default_url, "http://127.0.0.1:8080/");
		CHECK(c.components.size(), 2);

		CHECKS(c.components[0].name, "example");
		CHECKT(c.components[0].documentation == vector<string>{"doc"});
		CHECKT(c.components[0].source_files == vector<string>{"src"});
		CHECKT(c.components[0].test_files == vector<string>{"test"});
		CHECKS(c.components[0].url, "http://127.0.0.1:8080/");

		CHECKS(c.components[1].name, "experiment");
		CHECKT(c.components[1].documentation == vector<string>{"docs"});
		CHECKT(c.components[1].source_files == vector<string>{"source"});
		CHECKT(c.components[1].test_files == vector<string>{"tests"});
		CHECKS(c.components[1].url, "http://127.0.0.1:8080/");

		fs::remove(cfg);
		cout << endl << "END: read_yaml()" << endl;
	}

public:
	void test_list() override
	{
		create_json();
		create_yaml();
		read_json();
		read_yaml();
	}
};

int main()
{
	CONFIG_TEST ct;
	return ct.run();
}