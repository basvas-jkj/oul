module;

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#undef ERROR

export module server;
export import :base;
import :tools;

namespace fs = boost::filesystem;
namespace oul
{
	/// @brief Chyba: neznámý nebo nepodporovaný klient.
	export class UnknownClient: public CommonException
	{
	public:
		UnknownClient(): CommonException(unknown_client)
		{}
	};
	/// @brief Vybere vhodného klienta pro danou url.
	/// @param url Adresa serveru nebo cesta. 
	/// @return Ukazatel na instanci klienta.
	export client_ptr select_client(cr<string> url, cr<fs::path> component_location)
	{
		try
		{
			string pattern = "[ABCDEFGHIJKLMNOPQRSTUVXYZ]:[/\\]*";
			if (url.starts_with("http://") || url.starts_with("ftp://"))
			{
				return make_unique<CURL>(url, component_location);
			}
			else if (url.starts_with("scp://"))
			{
				return make_unique<SCP>(url, component_location);
			}
			else if (url.starts_with("file://"))
			{
				return make_unique<LOCAL>(url.substr(7), component_location);
			}
			else if (url[0] == '/' || url[0] == '\\' || match(url, pattern))
			{
				fs::create_directories(url);
				return make_unique<LOCAL>(url, component_location);
			}
		}
		catch (...)
		{}

		throw UnknownClient();
	}
}
