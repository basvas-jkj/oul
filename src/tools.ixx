module;

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#undef ERROR

export module tools;

import std;
import common;

using namespace std;
using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
	export template <class ...T>
	concept CmdArgs = ((convertible_to<T, string> || convertible_to<T, path>) && ...);

	/// @brief Spustí externí nástroj.
	/// @tparam ...T - std::string nebo boost::filesystem::path
	/// @param tool_path - cesta nástroje
	/// @param working_directory - složka, ve které se nástroj spustí
	/// @param ...args - argumenty předané nástroji
	/// @return True, pokud nástroj úspěšně doběhl. False v opačném případě.
	export template <CmdArgs ...T>
	bool call_tool(cr<path> tool_path, cr<path> working_directory, T&&... args)
	{
		child ch(tool_path, forward<T>(args)..., start_dir(working_directory), std_out > null, std_err > null);
		ch.wait();
		return ch.exit_code() == 0;
	}
	/// @brief Spustí zipovací nástroj v aktuálním pracovním adresáři.
	/// @tparam ...T - std::string nebo boost::filesystem::path 
	/// @param tool_path - cesta nástroje 
	/// @param ...args - argumenty předané nástroji 
	/// @return True, pokud nástroj úspěšně doběhl. False v opačném případě.
	export template <CmdArgs ...T>
	bool call_tool_here(cr<path> tool_path, T&&... args)
	{
		return call_tool(tool_path, current_path(), args...);
	}
	/// @brief Najde cestu k externímu nástroji.
	/// @param tool_name - jméno nástroje
	/// @return Pokud byl nástroj nalezen, vrátí cestu k němu. Jinak vrátí prázdnou cestu.
	export path find_tool(cr<string> tool_name)
	{
		return search_path(tool_name);
	}
}
