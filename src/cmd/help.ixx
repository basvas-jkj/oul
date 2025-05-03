export module args:help;

import :command;

namespace oul::args
{
    /// @brief Reprezentuje příkaz help - vypsání nápovědy programu OUL.
    export class HELP: public COMMAND
    {
    public:
        /// @brief Spustí příkaz help programu OUL.
        void run() const override
        {
            print_help();
        }
    };
}
