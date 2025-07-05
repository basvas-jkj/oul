#define BOOST_TEST_MODULE file_terator_test
#include <boost/filesystem/path.hpp>
#include <boost/test/included/unit_test.hpp>
#include <vector>

import file_iterator;
import mock_filesystem;

using namespace std;
using namespace boost::test_tools;
namespace fs = boost::filesystem;

namespace oul
{
	vector<fs::path> compute_result(cr<vector<mock_directory_entry>> filesystem,
									cr<file_map> include, cr<file_map> exclude)
	{
		FILE_ITERATOR fi(filesystem.begin(), filesystem.end(), include, exclude);
		return vector<fs::path>(fi.begin(), fi.end());
	}

#pragma region empty_filesystem_tests
	BOOST_AUTO_TEST_CASE(empty_file_system__nothing_included)
	{
		file_map include = {};
		file_map exclude = {};
		vector<fs::path> expected = {};
		vector<fs::path> result = compute_result(empty_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(empty_file_system__something_included)
	{
		file_map include = {{"src", {"file", "directory", "?pattern*"}}};
		file_map exclude = {};
		vector<fs::path> expected = {};
		vector<fs::path> result = compute_result(empty_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
#pragma endregion

#pragma region plain_filesystem_tests
	BOOST_AUTO_TEST_CASE(plain_filesystem__cpp_included)
	{
		file_map include = {{"src", {".*\\.cpp"}}};
		file_map exclude = {};
		vector<fs::path> expected = {"args.cpp", "command.cpp", "main.cpp"};
		vector<fs::path> result = compute_result(plain_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__args__both_cpp_hpp)
	{
		file_map include = {{"src", {"args\\.[ch]pp"}}};
		file_map exclude = {};
		vector<fs::path> expected = {"args.cpp", "args.hpp"};
		vector<fs::path> result = compute_result(plain_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__command__both_cpp_hpp)
	{
		file_map include = {{"src", {"command\\..?pp"}}};
		file_map exclude = {};
		vector<fs::path> expected = {"command.cpp", "command.hpp"};
		vector<fs::path> result = compute_result(plain_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__both_readmes)
	{
		file_map include = {{"src", {"README(\\.md|)"}}};
		file_map exclude = {};
		vector<fs::path> expected = {"README.md", "README"};
		vector<fs::path> result = compute_result(plain_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__same_include_exclude)
	{
		file_map include = {{"include", {".*\\.hpp"}}, {"doc", {"README\\.md"}}};
		file_map exclude = {{"doc", {".*\\.md"}}, {"include", {"args\\.hpp", "command\\.hpp"}}};
		vector<fs::path> expected = {};
		vector<fs::path> result = compute_result(empty_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__nothing_included_something_excluded)
	{
		file_map include = {};
		file_map exclude = {{"any", {".*\\..?pp", "README"}}};
		vector<fs::path> expected = {};
		vector<fs::path> result = compute_result(empty_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__everything_included_headers_excluded)
	{
		file_map include = {{"any", {".*\\..?pp", "README.*"}}};
		file_map exclude = {{"any", {".*\\.hpp"}}};
		vector<fs::path> expected = {"args.cpp", "command.cpp", "main.cpp", "README.md", "README"};
		vector<fs::path> result = compute_result(plain_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(plain_filesystem__unrelated_include_exclude)
	{
		file_map include = {{"include", {".*\\.hpp"}}};
		file_map exclude = {{"exclude", {".*\\.hpp"}}};
		vector<fs::path> expected = {"args.hpp", "command.hpp"};
		vector<fs::path> result = compute_result(plain_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
#pragma endregion

#pragma region structured_filesystem_test
	BOOST_AUTO_TEST_CASE(structured_filesystem__include_directories)
	{
		file_map include = {{"src", {"config", "empty", "server/zip"}}};
		file_map exclude = {};
		vector<fs::path> expected = {"config/config.cpp", "config/item.ixx",
									 "server/zip/zip_base.ixx", "server/zip/zip_tools.cpp"};
		vector<fs::path> result = compute_result(structured_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(structured_filesystem__include_exclude_directories)
	{
		file_map include = {{"src", {"config", "server"}}};
		file_map exclude = {{"src", {"config/.*\\.cpp", "server/zip"}}};
		vector<fs::path> expected = {"config/item.ixx", "server/server_base.ixx",
									 "server/server_tools.cpp"};
		vector<fs::path> result = compute_result(structured_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(structured_filesystem__split_all_files_into_two_groups)
	{
		file_map include = {{"cpp", {".*"}}, {"ixx", {".*"}}};
		file_map exclude = {{"cpp", {".*\\.ixx"}}, {"ixx", {".*\\.cpp"}}};
		vector<fs::path> expected = {
			"config/config.cpp",	   "config/item.ixx",		   "main.cpp",
			"server/zip/zip_base.ixx", "server/zip/zip_tools.cpp", "server/server_base.ixx",
			"server/server_tools.cpp"};
		vector<fs::path> result = compute_result(structured_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(structured_filesystem__two_groups_one_with_all_files)
	{
		file_map include = {{"all", {".*"}}, {"server", {"server"}}};
		file_map exclude = {{"server", {"server/zip"}}};
		vector<fs::path> expected = {
			"config/config.cpp",	   "config/item.ixx",		   "main.cpp",
			"server/zip/zip_base.ixx", "server/zip/zip_tools.cpp", "server/server_base.ixx",
			"server/server_tools.cpp"};
		vector<fs::path> result = compute_result(structured_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
	BOOST_AUTO_TEST_CASE(
		structured_filesystem__both_include_exclude_same_directory_different_groups)
	{
		file_map include = {{"zip", {"server/zip"}}, {"server", {"server"}}};
		file_map exclude = {{"server", {"server/zip"}}};
		vector<fs::path> expected = {"server/zip/zip_base.ixx", "server/zip/zip_tools.cpp",
									 "server/server_base.ixx", "server/server_tools.cpp"};
		vector<fs::path> result = compute_result(structured_filesystem, include, exclude);

		BOOST_TEST(result == expected, boost::test_tools::per_element());
	}
#pragma endregion
}
