module;

#include <string>
#include <boost/filesystem/operations.hpp>

export module args:install;

import :command;
import zip;

using namespace std;

namespace fs = boost::filesystem;
namespace oul::args
{
    /// @brief Reprezentuje příkaz install - stažení komponenty ze serveru.
    export class INSTALL: public COMMAND
    {
        /// @brief Zkontroluje argumenty příkazové řádky předané programu OUL.
        /// @return <code>true</code>, pokud argumenty byly předány správně, jinak <code>false</code>
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
    public:
        /// @brief Spustí příkaz install programu OUL.
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
                url = (fs::path(url) / server_name).generic_string();
            }

            string where = opt.get("-w");
            if (where == "")
            {
                where = local_name;
            }

            COMPONENT_MANAGER manager(std::move(cfg));
            manager.install(server_name, local_name, url, where);
        }
    };
}
