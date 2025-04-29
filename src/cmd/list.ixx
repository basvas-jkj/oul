export module args:list;

import :command;
import message;

export namespace oul::args
{
    class LIST: public COMMAND
    {
    public:
        void run() const override
        {
            CONFIG c = read_configuration();
            print_component_list(c.components);
        }
    };
}
