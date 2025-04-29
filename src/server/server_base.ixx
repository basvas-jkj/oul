module;

#include <boost/filesystem.hpp>

export module server:base;
import tools;
import common;
import config;
import message;
import file_iterator;

using namespace std;
namespace fs = boost::filesystem;
namespace oul
{
    /// @brief Chyba: nenalezen žádaný externí klient.
    export class MissingClient: public CommonException
    {
    public:
        MissingClient(): CommonException(ERROR::missing_client)
        {}
    };
    /// @brief Chyba: spuštění klienta archovu selhalo
    export class ClientError: public CommonException
    {
    public:
        ClientError(): CommonException(ERROR::client_error)
        {}
    };
    /// @brief Základní typ klientů.
    export class CLIENT
    {
    protected:
        string url;
        fs::path component_location;
        fs::path client_path;

        CLIENT(cr<string> url, cr<fs::path> cl): url(url), component_location(cl)
        {}
        CLIENT(cr<string> url, cr<fs::path> cl, cr<string> client_name):
            url(url), component_location(cl)
        {
            client_path = find_tool(client_name);
            if (client_path == "")
            {
                throw MissingClient();
            }
        }

        template <CmdArgs ...T>
        void call_client(T&&... args)
        {
            bool success = call_tool_here(client_path, args...);
            if (!success)
            {
                throw ClientError();
            }
        }

        virtual void upload(cr<string> url, cr<fs::path> file) = 0;
        virtual void download(cr<string> url, cr<fs::path> where) = 0;
    public:
        virtual void upload(cr<ITEM> component)
        {
            fs::create_directories(url);
            fs::path json_path = fs::path(url) / "oul.component.json";
            ofstream out(json_path.string());
            save_json(component, out);
            out.close();

            FILE_ITERATOR it(component_location, component.include, component.exclude);
            for (cr<fs::path> file : it)
            {
                upload(url, file);
            }
        }
        virtual ITEM download()
        {
            fs::create_directories(component_location);
            fs::path json_path = fs::path(url) / "oul.component.json";
            ITEM component = load_component(json_path.string());

            FILE_ITERATOR it(url, component.include, component.exclude);


            for (cr<fs::path> file : it)
            {
                download(url, file);
            }
            return component;
        }
        virtual ~CLIENT() = default;
    };
    export using client_ptr = unique_ptr<CLIENT>;
}
