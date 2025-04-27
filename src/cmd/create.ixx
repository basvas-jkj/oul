module;

#include <string>
#include <optional>

export module args:create;

import component_manager;
import :command;

export namespace oul::args
{
    class CREATE: public COMMAND
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
            string location = opt.get("-w");
            if (location == "")
            {
                location = name;
            }

            optional<CONFIG> c = read_configuration();

            if (!c.has_value())
            {
                return;
            }
            else if (c->contains(name))
            {
                report_error(ERROR::component_already_exists);
                return;
            }

            COMPONENT_MANAGER manager(move(*c));
            manager.create(name, location);
        }
    };
}
