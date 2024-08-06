#pragma once

#include <ranges>
#include <string>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

namespace oul
{
    template <class T>
    using optional = std::pair<bool, T>;

    struct ITEM
    {
        std::string name;
        std::string url;
        std::vector<std::string> documentation;
        std::vector<std::string> source_files;
        std::vector<std::string> test_files;
    };
    struct CONFIG
    {
        std::string location;
        std::string name;
        std::string default_url;
        std::vector<ITEM> components;

        CONFIG() {}
        CONFIG(const std::string& l): location(l)
        {}
        ~CONFIG();

        static std::string find();
        static optional<CONFIG> read();
        static optional<CONFIG> read(const std::string&);
        static void initialize();

        void add_component(const ITEM& i);
        void list_components();
        auto get_components()
        {
            return components | std::views::transform([](ITEM& i){return i.name; });
        }
        bool contains(const std::string& name);

        std::string get_url(const std::string&) const;
    };
}