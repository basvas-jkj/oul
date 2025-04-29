module;

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#undef ERROR

export module server:tools;
export import :base;
import tmp;
import zip;
import tools;
import common;

namespace fs = boost::filesystem;
namespace oul
{
    export class SCP: public CLIENT
    {
    protected:
        void upload(cr<string> url, cr<fs::path> file) override
        {
            call_client(url, file);
        }
        void download(cr<string> url, cr<fs::path> file) override
        {
            call_client(file, url);
        }
    public:
        SCP(cr<string> url, cr<fs::path> base): CLIENT(url, base, "scp")
        {}
    };
    export class CURL: public CLIENT
    {
        zip_tool_ptr tool;

    protected:
        void upload(cr<string> url, cr<fs::path> file) override
        {
            string post_argument = format("@{}", file.generic_string());
            call_client("---data-binary", post_argument, url);
        }
        void download(cr<string> url, cr<fs::path> file) override
        {
            call_client("-o", file, url);
        }
    public:
        void upload(cr<ITEM> component) override
        {
            TMP_FILE file = tool->zip(component, component_location.string());
            upload(url, file.get_path());
        }
        ITEM download() override
        {
            TMP_FILE file("%%%%-%%%%-%%%%-%%%%.zip", false);
            download(url, file.get_path());
            return tool->unzip(file, component_location.string());
        }
        CURL(cr<string> url, cr<fs::path> base): CLIENT(url, base, "curl")
        {
            tool = select_zip_tool();
        }
    };
    export class LOCAL: public CLIENT
    {
    protected:
        void upload(cr<string> url, cr<fs::path> file) override
        {
            fs::path entry = fs::relative(file, component_location);
            copy_file(component_location, url, entry);
        }
        void download(cr<string> url, cr<fs::path> file) override
        {
            fs::path entry = fs::relative(file, url);
            copy_file(url, component_location, entry);
        }
    public:
        LOCAL(cr<string> url, cr<fs::path> base): CLIENT(url, base)
        {}
    };
}
