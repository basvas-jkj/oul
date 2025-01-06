export module config;

import <iostream>;
import <ranges>;
import <boost/filesystem.hpp>;

import <yaml-cpp/yaml.h>;
import <nlohmann/json.hpp>;

import usings;
import data_file;

using namespace std;
using namespace nlohmann;
using namespace YAML;

namespace fs = boost::filesystem;
export namespace oul
{
    using file_map = map<string, vector<string>>;

    /**
     * @brief Struktura pro ukládání konfigurace komponenty.
     **/
    struct ITEM
    {
        string name;
        string original_name;
        string location;
        string url;

        file_map include;
        file_map exclude;

        /**
         * @brief Konstruktor vytvářející objekt struktury <code>ITEM</code>.
         * @param name jméno komponenty
         * @param location umístění komponenty
         **/
        ITEM(cr<string> name, cr<string> location): name(name), location(location)
        {}
         /**
         * @brief Konstruktor vytvářející objekt struktury <code>ITEM</code>.
         * @param component Konfigurace komponenty.
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
        /**
         * @brief Převede konfiguraci komponenty do formátu JSON.
         * @return konfigurace komponenty ve formátu JSON
         **/
        ordered_json json() const
        {
            ordered_json component;

            component["name"] = name;
            component["original_name"] = original_name;
            component["url"] = url;
            component["location"] = location;
            component["include"] = include;
            component["exclude"] = exclude;

            return component;
        }
        /**
         * @brief Převede konfiguraci komponenty do formátu YAML.
         * @return konfigurace komponenty ve formátu YAML
         **/
        Node yaml() const
        {
            Node component;

            component["name"] = name;
            component["original_name"] = original_name;
            component["url"] = url;
            component["location"] = location;;
            component["include"] = include;
            component["exclude"] = exclude;

            return component;
        }
    };

    /**
     * @brief Struktura pro ukládání konfigurace projektu.
     **/
    struct CONFIG
    {
    private:
        /**
         * @brief Uloží konfiguraci ve formátu JSON.
         **/
        void save_json() const
        {
            ordered_json root;

            root["project_name"] = project_name;
            root["default_url"] = default_url;

            for (cr<ITEM> i : components)
            {
                root["components"].push_back(i.json());
            }
            
            save(location, root);
        }
        /**
         * @brief Uloží konfiguraci ve formátu YAML.
         **/
        void save_yaml() const
        {
            Node root;

            root["project_name"] = project_name;
            root["default_url"] = default_url;

            for (cr<ITEM> i : components)
            {
                root["components"].push_back(i.yaml());
            }

            save(location, root);
        }

    public:
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
         * @param l umístění konfigurace v souborovém systému
         **/
        CONFIG(cr<string> l): location(l)
        {
            Node root = load(location);

            if (!root.IsNull())
            {
                project_name = root["project_name"].as<string>();
                default_url = root["default_url"].as<string>();

                for (cr<Node> component : root["components"])
                {
                    components.emplace_back(component);
                }
            }
            else
            {
                location = "";
            }
        }
        /**
         * @brief Zajišťuje zápsání změněné konfigurace do souboru.
         **/
        ~CONFIG()
        {
            if (location.ends_with(".json"))
            {
                save_json();
            }
            else if (location.ends_with(".yaml"))
            {
                save_yaml();
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
            cfg.location = move(location);
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
                    current_path(current);
                    return config.string();
                }

                config = current / "oul.config.yaml";
                if (fs::exists(config))
                {
                    current_path(current);
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
        auto get_component_names()
        {
            auto item_to_name = [](cr<ITEM> i) { return i.name; };
            return components | views::transform(item_to_name);
        }
        /**
         * @brief Přidá komponentu do konfigurace. 
         * @param i konfigurace komponenty
         **/
        void add_component(cr<ITEM> i)
        {
            components.push_back(i);
        }
        /**
         * @brief Zjistí, zda konfigurace projektu obsahuje komponentu daného jména.
         * @param name jméno komponenty
         * @return Je v konfiguraci přítomná komponenta?
         **/
        bool contains(cr<string> name)
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
         * @param name jméno komponenty
         * @return Platný iterátor ukazující na komponentu, pokud je v konfiguraci přítomná. Jinak vrátí <code>end()</code>
         **/
        vector<ITEM>::iterator get_component(cr<string> name)
        {
            return ranges::find_if(components, [name](cr<ITEM> i) { return i.name == name; });
        }

        /**
         * @brief Získá výchozí URL pro stažení komponenty.
         * @param component_name jméno komponenty
         * @return spojení výchozí URL projektu a jména komponenty
         **/
        string get_url(cr<string> component_name) const
        {
            return default_url + "/" + component_name;
        }
    };
}