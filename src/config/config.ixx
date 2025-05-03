module;

#include <ranges>
#include <iostream>
#include <optional>
#include <yaml-cpp/node/node.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

export module config;

import common;
import message;
export import item;
export import :data_file;

using namespace std;
namespace fs = boost::filesystem;

namespace oul
{
    /**
     * @brief Chyba v konfiguraci.
     **/
    export class InvalidConfiguration: public CommonException
    {
    public:
        InvalidConfiguration(ERROR name): CommonException(name)
        {}
    };

    /**
     * @brief Struktura pro ukládání konfigurace projektu.
     **/
    export struct CONFIG
    {
        string location;
        string project_name;
        string default_url;
        vector<ITEM> components;

        /**
         * @brief Konstruktor vytvářející prázdný objekt structury <code>CONFIG</code>.
         **/
        CONFIG() {}
        /**
         * @brief Konstruktor vytvářející objekt structury <code>CONFIG</code>.
         * Současně také načte konfiguraci ze souboru a zvaliduje.
         * @param l - umístění konfigurace v souborovém systému
         **/
        CONFIG(cr<string> l): location(l)
        {
            Node root = load(location);
            assert(!root.IsNull());

            project_name = root["project_name"].as<string>();
            default_url = root["default_url"].as<string>();

            for (cr<Node> component : root["components"])
            {
                components.emplace_back(component);
            }
        }
        CONFIG(cr<CONFIG>) = delete;
        CONFIG(CONFIG&&) = default;
        /**
         * @brief Zajišťuje zápsání změněné konfigurace do souboru.
         **/
        ~CONFIG() noexcept
        {
            try
            {
                if (location.ends_with(".json"))
                {
                    save_json(*this);
                }
                else if (location.ends_with(".yaml"))
                {
                    save_yaml(*this);
                }
            }
            catch (...)
            {
                report_error(ERROR::unexpected_error);
            }
        }

        /**
         * @brief Vytvoří prázdnou konfiguraci. Potřebné údaje získá interaktivně ze standardního vstupu.
         **/
        static void initialize()
        {
            string name;
            cout << "Enter the project name: ";
            cin >> name;

            string format;
            cout << "Do you prefer JSON or YAML for the configuration? (j/y): ";
            cin >> format;

            string default_url;
            cout << "Enter URL used by default during installing components: ";
            cin >> default_url;

            string location = (format[0] == 'j' || format[0] == 'J') ? "oul.config.json" : "oul.config.yaml";
            location = (fs::current_path() / location).string();

            CONFIG cfg;
            cfg.location = std::move(location);
            cfg.project_name = name;
            cfg.default_url = default_url;
        }
        /**
         * @brief Najde cestu ke konfiguračnímu souboru.Pokud je soubor nalezen, nastaví pracovní složku na jeho umístění.
         * @return Pokud byla konfigurace nalezena, vrátí cestu. Jinak vrátí "".
         **/
        static optional<string> find()
        {
            fs::path current = fs::current_path();
            do
            {
                fs::path config = current / "oul.config.json";
                if (fs::exists(config))
                {
                    return config.string();
                }

                config = current / "oul.config.yaml";
                if (fs::exists(config))
                {
                    return config.string();
                }

                current = current.parent_path();
            }
            while (!equivalent(current, current.root_path()));
            return nullopt;
        }

        /**
         * @brief Převede seznam všech komponent v konfiguraci na seznam názvů.
         * @return iterovatelný seznam (<code>view</code>) názvů všech komponent
         **/
        auto get_component_names() const
        {
            auto item_to_name = [](cr<ITEM> i) { return i.name; };
            return components | views::transform(item_to_name);
        }
        /**
         * @brief Přidá komponentu do konfigurace.
         * @param i - konfigurace komponenty
         **/
        void add_component(cr<ITEM> i)
        {
            components.push_back(i);
        }
        /**
         * @brief Odebere komponentu z konfigurace.
         * @param name - jméno komponenty
         **/
        void remove_component(cr<string> name)
        {
            auto has_equal_name = [&name](ITEM& i)
                {
                    return i.name == name;
                };

            erase_if(components, has_equal_name);
        }
        /**
         * @brief Zjistí, zda konfigurace projektu obsahuje komponentu daného jména.
         * @param name - jméno komponenty
         * @return Je v konfiguraci přítomná komponenta?
         **/
        bool contains(cr<string> name) const
        {
            for (cr<string> component : get_component_names())
            {
                if (component == name)
                {
                    return true;
                }
            }
            return false;
        }
        /**
         * @brief Najde v konfiguraci projektu komponentu a vrátí iterátor.
         * @param name - jméno komponenty
         * @return Platný iterátor ukazující na komponentu, pokud je v konfiguraci přítomná. Jinak vrátí <code>end()</code>
         **/
        vector<ITEM>::iterator get_component(cr<string> name)
        {
            return ranges::find_if(components, [name](cr<ITEM> i) { return i.name == name; });
        }

        /**
         * @brief Získá výchozí URL pro stažení komponenty.
         * @param component_name - jméno komponenty
         * @return spojení výchozí URL projektu a jména komponenty
         **/
        string get_url(cr<string> component_name) const
        {
            return (fs::path(default_url) / component_name).generic_string();
        }
        fs::path get_directory() const
        {
            return fs::path(location).parent_path();
        }
    };
}
