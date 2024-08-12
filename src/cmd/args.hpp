#pragma once

#include <map>
#include <span>
#include <deque>
#include <vector>
#include <string>

namespace oul
{
	/**
	 * @brief Vypíše stručnou nápovědu pro každý podporovaný příkaz.
	 **/
	void write_short_help();
	/**
	 * @brief Převede přepínač na jeho krátkou verzi.
	 * @param opt dlouhá veze přepínače
	 * @return Krátká verze přepínače, pokud existuje. Jinak vrátí původní verzi.
	 **/
	std::string convert_to_short(const std::string& opt);

	/**
	 * @brief Třída ukládající informace o jednotlivých příkazech a spravuje svoje vlastní objekty.
	 **/
	class COMMAND
	{
		static int next_index;
		static std::vector<COMMAND> list;
		static std::map<std::string, COMMAND&> dictionary;

		int index;
		std::string name;
		std::string short_help;

		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>COMMAND</code>.
		 * @param name Jméno příkazu.
		 * @param short_help Nápověda tohoto příkazu.
		 **/
		COMMAND(const std::string& name, const std::string& short_help);

	public:
		/**
		 * @brief Připraví třídní členy k používání. (Nahrazuje statický konstruktor.)
		 **/
		static void init();
		/**
		 * @brief Vrátí objekt reprezentující prázdný (nezadaný) příkaz.
		 * @return objekt prázdného příkazu
		 **/
		static COMMAND& none();
		/**
		 * @brief Vyhledá příkaz podle jména.
		 * @param name Jméno hledaného příkazu.
		 * @return Vrátí objekt příkazu příslušného jména. Pokud takový objekt neexistuje, vrátí <code>none</code>
		 **/
		static COMMAND& find(const std::string& name);
		/**
		 * @brief Vrátí seznam všech dostupných příkazů.
		 * @return seznam všech dostupných příkazů
		 **/
		static std::span<COMMAND> all();
		/**
		 * @brief Vrátí jméno tohoto příkazu.
		 * @return Jméno tohoto příkazu.
		 **/
		std::string get_name() const;
		/**
		 * @brief Získá text nápovědy k tomuto příkazu
		 * @return Krátkou nápovědu k tomuto příkazu.
		 **/
		std::string get_short_help() const;
		/**
		 * @brief Porovná dva objekty typu <code>COMMAND</code>.
		 * @param o Jiný objekt typu <code>COMMAND</code>.
		 * @return Reprezentují oba objekty stejný příkaz?
		 **/
		bool operator==(const COMMAND& o) const;
		/**
		 * @brief Zkontoluje, zda se jedná o zadaný příkaz.
		 * @param name Jméno příkazu.
		 * @return Reprezentuje tento objekt příkaz se zadaným jménem?
		 **/
		bool is(const std::string& name) const;
	};
	/**
	 * @brief Třída reprezentující argumenty příkazové řádky předané tomuto programu.
	 **/
	class ARGS
	{
		COMMAND* c;
		std::map<std::string, std::string> options;
		std::deque<std::string> arguments;

	public:
		/**
		 * @brief Konstruktor vytvářející objekty třídy <code>ARGS</code>.
		 * @param argc Počet argumentů příkazové řádky.
		 * @param argv Seznam argumentů příkazoé řádky.
		 **/
		ARGS(int argc, char* argv[]);
		/**
		 * @brief Vrací postupně všechny argumenty, které byly předány tomuto programu.
		 * @return První nepřečtený argument, který není přepínač ani příkaz. Pokud již byly všechny argumenty vráceny, vrátí "".
		 **/
		std::string next_arg();
		/**
		 * @brief Zkontoluje, se kterým příkazem byl tento program spuštěn.
		 * @param name Jméno příkazu.
		 * @return Byl program volán s daným příkazem?
		 **/
		bool is(const std::string& name) const;
		/**
		 * @brief Kontroluje přítomnost příslušného přepínače.
		 * @param option jméno žádaného přepínače
		 * @return Byl přepínač předán tomuto programu jako argument?
		 **/
		bool has_option(const std::string& option) const;
		/**
		 * @brief Vrátí hodnotu příslušného přepínače.
		 * @param option jméno žádaného přepínače
		 * @return Hodnotu daného přepínače, pokud byla zadána. Pokud byl přepínač použit bez hodnoty nebo není přítomen vůbec, vrátí "". 
		 **/
		std::string get_option(const std::string& option) const;
	};
}