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
	void create(const string& suffix)
	{
		cout << endl << format("BEGIN: create_{}()", suffix) << endl;
		test_cin(format("try {} http://127.0.0.1:8080", suffix));
		CONFIG::initialize();

		fs::path cfg = "oul.config." + suffix;
		CHECKT(fs::exists(cfg));
		CHECKT(compare_files(cfg, test_files / ("correct." +  suffix)));
		fs::remove(cfg);
		cout << endl << format("END: create_{}()", suffix) << endl;
	}
	void read(const string& suffix)
	{
		cout << endl << format("BEGIN: read_{}()", suffix) << endl;
		fs::copy_file(test_files / ("oul.config." + suffix), "./oul.config." + suffix);
		fs::path cfg = CONFIG::find();
		
		{
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
		}

		fs::remove(cfg);
		cout << endl << format("END: read_{}()", suffix) << endl;
	}

public:
	void test_list() override
	{
		create("json");
		create("yaml");
		read("json");
		read("yaml");
	}
};

int main()
{
	CONFIG_TEST ct;
	return ct.run();
}