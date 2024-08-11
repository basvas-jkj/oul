#pragma once

#include <map>
#include <span>
#include <deque>
#include <vector>
#include <string>

namespace oul
{
	/**
	 * @brief Vypíše struènou nápovìdu pro každý podporovaný pøíkaz.
	 **/
	void write_short_help();

	/**
	 * @brief Tøída ukládající informace o jednotlivých pøíkazech a spravuje svoje vlastní objekty.
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
		 * @brief Konstruktor vytváøející objekty tøídy <code>COMMAND</code>.
		 * @param name Jméno pøíkazu.
		 * @param short_help Nápovìda tohoto pøíkazu.
		 **/
		COMMAND(const std::string& name, const std::string& short_help);

	public:
		/**
		 * @brief Pøipraví tøídní èleny k používání. (Nahrazuje statický konstruktor.)
		 **/
		static void init();
		/**
		 * @brief Vrátí objekt reprezentující prázdný (nezadaný) pøíkaz.
		 * @return objekt prázdného pøíkazu
		 **/
		static COMMAND& none();
		/**
		 * @brief Vyhledá pøíkaz podle jména.
		 * @param name Jméno hledaného pøíkazu.
		 * @return Vrátí objekt pøíkazu pøíslušného jména. Pokud takový objekt neexistuje, vrátí <code>none</code>
		 **/
		static COMMAND& find(const std::string& name);
		/**
		 * @brief Vrátí seznam všech dostupných pøíkazù.
		 * @return seznam všech dostupných pøíkazù
		 **/
		static std::span<COMMAND> all();
		/**
		 * @brief Vrátí jméno tohoto pøíkazu.
		 * @return Jméno tohoto pøíkazu.
		 **/
		std::string get_name() const;
		/**
		 * @brief Získá text nápovìdy k tomuto pøíkazu
		 * @return Krátkou nápovìdu k tomuto pøíkazu.
		 **/
		std::string get_short_help() const;
		/**
		 * @brief Porovná dva objekty typu <code>COMMAND</code>.
		 * @param o Jiný objekt typu <code>COMMAND</code>.
		 * @return Reprezentují oba objekty stejný pøíkaz?
		 **/
		bool operator==(const COMMAND& o) const;
		/**
		 * @brief Zkontoluje, zda se jedná o zadaný pøíkaz.
		 * @param name Jméno pøíkazu.
		 * @return Reprezentuje tento objekt pøíkaz se zadaným jménem?
		 **/
		bool is(const std::string& name) const;
	};
	/**
	 * @brief Tøída reprezentující argumenty pøíkazové øádky pøedané tomuto programu.
	 **/
	class ARGS
	{
		COMMAND* c;
		std::map<std::string, std::string> options;
		std::deque<std::string> arguments;

	public:
		/**
		 * @brief Konstruktor vytváøející objekty tøídy <code>ARGS</code>.
		 * @param argc Poèet argumentù pøíkazové øádky.
		 * @param argv Seznam argumentù pøíkazoé øádky.
		 **/
		ARGS(int argc, char* argv[]);
		/**
		 * @brief Vrací postupnì všechny argumenty, které byly pøedány tomuto programu.
		 * @return První nepøeètený argument, který není pøepínaè ani pøíkaz. Pokud již byly všechny argumenty vráceny, vrátí "".
		 **/
		std::string next_arg();
		/**
		 * @brief Zkontoluje, se kterým pøíkazem byl tento program spuštìn.
		 * @param name Jméno pøíkazu.
		 * @return Byl program volán s daným pøíkazem?
		 **/
		bool is(const std::string& name) const;
		/**
		 * @brief Kontroluje pøítomnost pøíslušného pøepínaèe.
		 * @param option jméno žádaného pøepínaèe
		 * @return Byl pøepínaè pøedán tomuto programu jako argument?
		 **/
		bool has_options(const std::string& option) const;
		/**
		 * @brief Vrátí hodnotu pøíslušného pøepínaèe.
		 * @param option jméno žádaného pøepínaèe
		 * @return Hodnotu daného pøepínaèe, pokud byla zadána. Pokud byl pøepínaè použit bez hodnoty nebo není pøítomen vùbec, vrátí "". 
		 **/
		std::string get_option(const std::string& option) const;
	};
}