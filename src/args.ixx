module;

#include <tuple>
#include <memory>
#include <string>
#include <vector>

export module args;

import usings;
import message;

import :init;
import :create;
import :_delete;

import :group_add;
import :group_remove;
import :add;
import :remove;
import :exclude;

import :list;
import :rename;
import :upload;
import :install;
export import :command;

using namespace std;
using namespace oul;
using namespace oul::args;

namespace oul
{
    static tuple<string, string> parse_option(cr<string> arg)
    {
        size_t index = arg.find('=');

        if (index == string::npos)
        {
            return tuple(arg, "");
        }
        else
        {
            string name = arg.substr(0, index);
            string value = arg.substr(index + 1);
            return tuple(move(name), move(value));
        }
    }
    export unique_ptr<COMMAND> read_args(int argc, char* argv[])
    {
        vector<string> args(argv + 1, argv + argc);

        int start = 1;
        unique_ptr<COMMAND> command;
        if (args.size() == 0)
        {
            report_error(missing_command);
            return nullptr;
        }
        if (args[0] == "init")
        {
            command = make_unique<INIT>();
        }
        else if (args[0] == "create")
        {
            command = make_unique<CREATE>();
        }
        else if (args[0] == "delete")
        {
            command = make_unique<DELETE>();
        }
        else if (args[0] == "add")
        {
            command = make_unique<ADD>();
        }
        else if (args[0] == "remove")
        {
            command = make_unique<REMOVE>();
        }
        else if (args[0] == "exclude")
        {
            command = make_unique<EXCLUDE>();
        }
        else if (args[0] == "list")
        {
            command = make_unique<LIST>();
        }
        else if (args[0] == "upload")
        {
            command = make_unique<UPLOAD>();
        }
        else if (args[0] == "install")
        {
            command = make_unique<INSTALL>();
        }
        else if (args[0] == "rename")
        {
            command = make_unique<RENAME>();
        }
        else if (args[0] == "group")
        {
            start += 1;
            if (args.size() == 1)
            {
                report_error(missing_group_subcommand);
                return nullptr;
            }
            else if (args[1] == "add")
            {
                command = make_unique<GROUP_ADD>();
            }
            else if (args[1] == "remove")
            {
                command = make_unique<GROUP_REMOVE>();
            }
            else
            {
                report_error(missing_group_subcommand);
                return nullptr;
            }
        }
        else
        {
            report_error(unknown_command);
            return nullptr;
        }

        OPTIONS opt;
        vector<string> arguments;

        for (int f = start; f < args.size(); f += 1)
        {
            string& arg = args[f];

            if (arg[0] == '-')
            {
                auto [name, value] = parse_option(arg);
                opt.add(name, value);
            }
            else
            {
                arguments.push_back(move(arg));
            }
        }

        command->init(move(opt), move(arguments));
        return command;
    }
}
