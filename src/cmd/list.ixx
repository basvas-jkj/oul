module;

#include <string>
#include <iostream>

export module args:list;

import :command;
import message;

using namespace std;

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
