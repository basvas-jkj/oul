export module args:help;

import :command;

export namespace oul::args
{
    class HELP: public COMMAND
    {
    public:
        void run() const override
        {
            print_help();
        }
    };
}
