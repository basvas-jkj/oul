#include <memory>

import args;
import common;
import message;
import component_manager;

using namespace std;
using namespace oul;

int main(int argc, char* argv[])
{
    init_messages(argv[0]);
    try
    {
        unique_ptr<COMMAND> c = read_args(argc, argv);

        if (c != nullptr && c->check())
        {
            c->run();
            return 0;
        }
    }
    catch (CommonException& e)
    {
        e.report();
    }
    catch (...)
    {
        report_error(ERROR::unexpected_error);
    }

    return 1;
}
