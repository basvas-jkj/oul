module;

#include <string>

export module args:install;

import :command;
import zip;

using namespace std;

export namespace oul::args
{
    class INSTALL: public COMMAND
    {
        bool check() const override
        {
            if (arguments.size() < 1)
            {
                report_error(missing_component_name);
                return false;
            }
            else
            {
                return true;
            }
        }
    public:
        void run() const override
        {
            CONFIG cfg = read_configuration();

            cr<string> server_name = arguments[0];
            string local_name = opt.get("-as");
            if (local_name == "")
            {
                local_name = server_name;
            }

            string url = opt.get("-url");
            if (url == "")
            {
                url = cfg.get_url(server_name);
            }
            else
            {
                url = url + "/" + server_name;
            }

            string where = opt.get("-w");
            if (where == "")
            {
                where = cfg.location;
            }

            COMPONENT_MANAGER manager(move(cfg));
            manager.install(server_name, local_name, url, where);
        }
    };
}
