export module args:list;

import :command;
import message;

namespace oul::args
{
    /// @brief Reprezentuje příkaz list - vypsání seznamu komponent v projektu.
    export class LIST: public COMMAND
    {
    public:
        /// @brief Spustí příkaz list programu OUL.
        void run() const override
        {
            CONFIG c = read_configuration();
            print_component_list(c.components);
        }
    };
}
