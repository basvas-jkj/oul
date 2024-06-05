#pragma once

#include <ranges>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

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

        static void read_json(CONFIG&, const nlohmann::json&);
        static std::optional<CONFIG> read_json(const std::string&);

        static void read_yaml(CONFIG&, const YAML::Node&);
        static std::optional<CONFIG> read_yaml(const std::string&);

        void add_component_json(const std::string&, const std::string&, const std::vector<std::string>&) const;
        void add_component_yaml(const std::string&, const std::string&, const std::vector<std::string>&) const;

        CONFIG(const std::string& l): location(l)
        {}

    public:
        CONFIG() {}
        static std::string find();
        static std::optional<CONFIG> read();
        static std::optional<CONFIG> read(const std::string&);
        static std::optional<CONFIG> read_json(const nlohmann::json&);
        static std::optional<CONFIG> read_yaml(const YAML::Node&);
        static void initialize();

        void add_component(const std::string&, const std::string&, const std::vector<std::string>&);
        void list_components();
        auto get_components()
        {
            return components | std::views::transform([](ITEM& i){return i.name; });
        }

        std::string get_url(const std::string&) const;
    };
}