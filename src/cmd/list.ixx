module;

#include <string>

export module args:list;

import :command;

using namespace std;

export namespace oul::args
{
    class LIST: public COMMAND
    {
    public:
        void run() const override
        {
            CONFIG c = read_configuration();

            println("List of components used in this project:");
            for (cr<ITEM> i : c.components)
            {
                cr<string> url = (i.url.empty()) ? "local only" : i.url;
                println("\t{}\t({})", i.name, url);
            }
        }
    };
}
