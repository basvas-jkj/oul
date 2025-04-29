module;

#include <string>

export module args:rename;

import :command;
import server;
import message;

using namespace std;

export namespace oul::args
{
    class RENAME: public COMMAND
    {
    public:
        bool check() const override
        {
            if (arguments.size() < 1)
            {
                report_error(ERROR::missing_component_name);
                return false;
            }
            else if (arguments.size() < 2)
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
            cr<string> original_name = arguments[0];
            cr<string> new_name = arguments[1];

            COMPONENT_MANAGER manager = open_manager();
            manager.rename(original_name, new_name);
        }
    };
}
