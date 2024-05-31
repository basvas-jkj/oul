#pragma once

#include <string>
#include <vector>
#include <optional>

namespace oul
{
    struct ITEM
    {
        std::string name;
        std::string url;
        std::vector<std::string> content;
    };
    class CONFIG
    {
        std::string location;
        std::string name;
        std::string default_url;
        std::vector<ITEM> components;

        static std::optional<CONFIG> read_json(const std::string&);
        static std::optional<CONFIG> read_yaml(const std::string&);

        void add_component_json(const std::string&, const std::string&, const std::vector<std::string>&) const;
        void add_component_yaml(const std::string&, const std::string&, const std::vector<std::string>&) const;

        CONFIG(const std::string& l): location(l)
        {}

    public:
        static std::string find();
        static std::optional<CONFIG> read();
        static std::optional<CONFIG> read(const std::string&);
        static void initialize();

        void add_component(const std::string&, const std::string&, const std::vector<std::string>&);
        void list_components();

        std::string get_url(const std::string&) const;
    };
}
