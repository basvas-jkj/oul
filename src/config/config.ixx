export module config;

import <iostream>;
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
    /**
     * @brief Třída pro ukládání konfigurace projektu.
     **/
    struct CONFIG
    {
    private:
        void save_json() const
        {
            ordered_json root;

            root["project_name"] = project_name;
            root["default_url"] = default_url;

            save(location, root);
        }
        void save_yaml() const
        {
            Node root;

            root["project_name"] = project_name;
            root["default_url"] = default_url;

            save(location, root);
        }

    public:
        string location;
        string project_name;
        string default_url;

        /**
         * @brief Konstruktor vytvářející prázdný objekt structury <code>CONFIG</code>.
         **/
        CONFIG() {}
        /**
         * @brief Konstruktor vytvářející objekt structury <code>CONFIG</code>.
         * Současně také přeč
         * @param l umístění konfigurace v souborovém systému
         **/
        CONFIG(cr<string> l): location(l)
        {
            Node root = load(location);

            if (!root.IsNull())
            {
                project_name = root["project_name"].as<string>();
                default_url = root["default_url"].as<string>();
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
            else
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
    };
}