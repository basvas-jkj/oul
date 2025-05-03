export module args:group_remove;

import :command;

namespace oul::args
{
    /// @brief Reprezentuje příkaz group remove - odebrání skupiny souborů.
    export class GROUP_REMOVE: public COMMAND
    {
    public:
        /// @brief Zkontroluje argumenty příkazové řádky předané programu OUL.
        /// @return <code>true</code>, pokud argumenty byly předány správně, jinak <code>false</code>
        bool check() const override
        {
            if (arguments.size() < 1)
            {
                report_error(ERROR::missing_component_name);
            }
            if (arguments.size() < 2)
            {
                report_error(ERROR::missing_group_name);
                return false;
            }
            else
            {
                return true;
            }
        }
        /// @brief Spustí příkaz group remove programu OUL.
        void run() const override
        {
            COMPONENT_MANAGER manager = open_manager();
            manager.remove_group(arguments[0], arguments[1]);
        }
    };
}
