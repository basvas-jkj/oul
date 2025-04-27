module;

#include <string>
#include <yaml-cpp/yaml.h>

export module item;
import common;

using namespace std;
using namespace YAML;
namespace oul
{
    /**
     * @brief Struktura pro ukládání konfigurace komponenty.
     **/
    export struct ITEM
    {
        string name;
        string original_name;
        string location;
        string url;

        file_map include;
        file_map exclude;

        /**
         * @brief Konstruktor vytvářející objekt struktury <code>ITEM</code>.
         * @param name - jméno komponenty
         * @param location - umístění komponenty
         **/
        ITEM(cr<string> name, cr<string> location): name(name), location(location)
        {}
        /**
         * @brief Konstruktor vytvářející objekt struktury <code>ITEM</code>.
         * @param component - Konfigurace komponenty.
         **/
        ITEM(cr<Node> component)
        {
            name = component["name"].as<string>();
            original_name = component["original_name"].as<string>();
            url = component["url"].as<string>();
            location = component["location"].as<string>();
            include = component["include"].as<file_map>();
            exclude = component["exclude"].as<file_map>();
        }
    };
}
