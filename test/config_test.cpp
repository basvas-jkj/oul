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
		test_cin("try Json http://127.0.0.1:8080");
		CONFIG::initialize();

		fs::path cfg = "oul.config.json";
		CHECKT(fs::exists(cfg));
		CHECKT(compare_files(cfg, test_files / "correct.json"));
		fs::remove(cfg);
	}
	void create_yaml()
	{
		test_cin("try Yaml http://127.0.0.1:8080");
		CONFIG::initialize();

		fs::path cfg = "oul.config.yaml";
		CHECKT(fs::exists(cfg));
		CHECKT(compare_files(cfg, test_files / "correct.yaml"));
		fs::remove(cfg);
	}

public:
	void test_list() override
	{
		create_json();
		create_yaml();
	}
};

int main()
{
	CONFIG_TEST ct;
	return ct.run();
}