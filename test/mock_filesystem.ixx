module;

#include <vector>
#include <boost/filesystem/path.hpp>

export module mock_filesystem;
export import usings;

using namespace std;
namespace fs = boost::filesystem;
export struct mock_directory_entry
{
    bool is_file;
    int d;
    fs::path p;

    bool is_regular_file() const
    {
        return is_file;
    }
    bool is_directory() const
    {
        return !is_file;
    }
    cr<fs::path> path() const
    {
        return p;
    }
    int depth() const
    {
        return d;
    }
};

mock_directory_entry make_file(int depth, fs::path&& p)
{
    return mock_directory_entry{true, depth, std::move(p)};
}
mock_directory_entry make_directory(int depth, fs::path&& p)
{
    return mock_directory_entry{false, depth, std::move(p)};
}

export vector<mock_directory_entry> empty_filesystem = {};
export vector<mock_directory_entry> plain_filesystem =
{
    make_file(1, "args.cpp"),
    make_file(1, "args.hpp"),
    make_file(1, "command.cpp"),
    make_file(1, "command.hpp"),
    make_file(1, "main.cpp"),
    make_file(1, "README.md"),
    make_file(1, "README")
};
export vector<mock_directory_entry> structured_filesystem =
{
    make_directory(1, "config"),
    make_file(2, "config/config.ixx"),
    make_file(2, "config/item.ixx"),
    make_directory(1, "empty"),
    make_file(1, "main.cpp"),
    make_directory(1, "server"),
    make_directory(2, "server/zip"),
    make_directory(3, "server/zip/zip_base.ixx"),
    make_directory(3, "server/zip/zip_tools.ixx"),
    make_file(2, "server/server_base.ixx"),
    make_file(2, "server/server_tools.ixx")
};
