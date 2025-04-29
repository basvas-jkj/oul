module;

#include <string>
#include <boost/filesystem/operations.hpp>

export module args:upload;

import :command;
import server;
import message;

using namespace std;

namespace oul::args
{
    /**
     * @brief Chyba: URL nebyla právně zadaná.
     **/
    export class MissingUrl: public CommonException
    {
    public:
        MissingUrl(ERROR name = ERROR::missing_url): CommonException(name)
        {}
    };

    class UPLOAD: public COMMAND
    {
    public:
        bool check() const override
        {
            if (arguments.size() < 1)
            {
                report_error(ERROR::missing_component_name);
                return false;
            }
            else
            {
                return true;
            }
        }
        void run() const override
        {
            cr<string> name = arguments[0];
            CONFIG cfg = read_configuration();
            vector<ITEM>::iterator i = cfg.get_component(name);
            if (i == cfg.components.end())
            {
                throw MissingComponent();
            }
            ITEM& component = *i;

            string server_name = opt.get("-as");
            if (server_name == "")
            {
                server_name = name;
                component.original_name = name;
            }

            string url = opt.get("-url");
            if (url == "")
            {
                if (component.url != "")
                {
                    url = component.url;
                }
                else
                {
                    throw MissingUrl();
                }
            }
            else if (component.url == "")
            {
                component.url = url;
            }

            url += "/" + server_name;

            client_ptr client = select_client(url, cfg.get_directory() / component.location);
            client->upload(component);
        }
    };
}
