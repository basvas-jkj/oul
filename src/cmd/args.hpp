#pragma once

#include <map>
#include <span>
#include <deque>
#include <vector>
#include <string>

namespace oul
{
	/**
	 * @brief Vyp�e stru�nou n�pov�du pro ka�d� podporovan� p��kaz.
	 **/
	void write_short_help();

	/**
	 * @brief T��da ukl�daj�c� informace o jednotliv�ch p��kazech a spravuje svoje vlastn� objekty.
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
		 * @brief Konstruktor vytv��ej�c� objekty t��dy <code>COMMAND</code>.
		 * @param name Jm�no p��kazu.
		 * @param short_help N�pov�da tohoto p��kazu.
		 **/
		COMMAND(const std::string& name, const std::string& short_help);

	public:
		/**
		 * @brief P�iprav� t��dn� �leny k pou��v�n�. (Nahrazuje statick� konstruktor.)
		 **/
		static void init();
		/**
		 * @brief Vr�t� objekt reprezentuj�c� pr�zdn� (nezadan�) p��kaz.
		 * @return objekt pr�zdn�ho p��kazu
		 **/
		static COMMAND& none();
		/**
		 * @brief Vyhled� p��kaz podle jm�na.
		 * @param name Jm�no hledan�ho p��kazu.
		 * @return Vr�t� objekt p��kazu p��slu�n�ho jm�na. Pokud takov� objekt neexistuje, vr�t� <code>none</code>
		 **/
		static COMMAND& find(const std::string& name);
		/**
		 * @brief Vr�t� seznam v�ech dostupn�ch p��kaz�.
		 * @return seznam v�ech dostupn�ch p��kaz�
		 **/
		static std::span<COMMAND> all();
		/**
		 * @brief Vr�t� jm�no tohoto p��kazu.
		 * @return Jm�no tohoto p��kazu.
		 **/
		std::string get_name() const;
		/**
		 * @brief Z�sk� text n�pov�dy k tomuto p��kazu
		 * @return Kr�tkou n�pov�du k tomuto p��kazu.
		 **/
		std::string get_short_help() const;
		/**
		 * @brief Porovn� dva objekty typu <code>COMMAND</code>.
		 * @param o Jin� objekt typu <code>COMMAND</code>.
		 * @return Reprezentuj� oba objekty stejn� p��kaz?
		 **/
		bool operator==(const COMMAND& o) const;
		/**
		 * @brief Zkontoluje, zda se jedn� o zadan� p��kaz.
		 * @param name Jm�no p��kazu.
		 * @return Reprezentuje tento objekt p��kaz se zadan�m jm�nem?
		 **/
		bool is(const std::string& name) const;
	};
	/**
	 * @brief T��da reprezentuj�c� argumenty p��kazov� ��dky p�edan� tomuto programu.
	 **/
	class ARGS
	{
		COMMAND* c;
		std::map<std::string, std::string> options;
		std::deque<std::string> arguments;

	public:
		/**
		 * @brief Konstruktor vytv��ej�c� objekty t��dy <code>ARGS</code>.
		 * @param argc Po�et argument� p��kazov� ��dky.
		 * @param argv Seznam argument� p��kazo� ��dky.
		 **/
		ARGS(int argc, char* argv[]);
		/**
		 * @brief Vrac� postupn� v�echny argumenty, kter� byly p�ed�ny tomuto programu.
		 * @return Prvn� nep�e�ten� argument, kter� nen� p�ep�na� ani p��kaz. Pokud ji� byly v�echny argumenty vr�ceny, vr�t� "".
		 **/
		std::string next_arg();
		/**
		 * @brief Zkontoluje, se kter�m p��kazem byl tento program spu�t�n.
		 * @param name Jm�no p��kazu.
		 * @return Byl program vol�n s dan�m p��kazem?
		 **/
		bool is(const std::string& name) const;
		/**
		 * @brief Kontroluje p��tomnost p��slu�n�ho p�ep�na�e.
		 * @param option jm�no ��dan�ho p�ep�na�e
		 * @return Byl p�ep�na� p�ed�n tomuto programu jako argument?
		 **/
		bool has_options(const std::string& option) const;
		/**
		 * @brief Vr�t� hodnotu p��slu�n�ho p�ep�na�e.
		 * @param option jm�no ��dan�ho p�ep�na�e
		 * @return Hodnotu dan�ho p�ep�na�e, pokud byla zad�na. Pokud byl p�ep�na� pou�it bez hodnoty nebo nen� p��tomen v�bec, vr�t� "". 
		 **/
		std::string get_option(const std::string& option) const;
	};
}