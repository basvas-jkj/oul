module;

#include <boost/filesystem/path.hpp>
#include <boost/process.hpp>
#include <regex>

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
		UnknownClient(): CommonException(MESSAGE::unknown_client) {}
	};
	/// @brief Vybere vhodného klienta pro danou url.
	/// @param url - Adresa serveru nebo cesta.
	/// @return Ukazatel na instanci klienta.
	export client_ptr select_client(cr<string> component_name, cr<string> url,
									cr<fs::path> component_location)
	{
		try
		{
			static regex pattern("[A-Z]:[\\/].*");
			if (url.starts_with("http://"))
			{
				return make_unique<CURL_HTTP>(component_name, url, component_location);
			}
			else if (url.starts_with("ftp://"))
			{
				return make_unique<CURL_FTP>(component_name, url, component_location);
			}
			else if (url.starts_with("scp://"))
			{
				return make_unique<SCP>(component_name, url, component_location);
			}
			else if (url.starts_with("file://"))
			{
				return make_unique<LOCAL>(component_name, url.substr(7), component_location);
			}
			else if (url[0] == '/' || url[0] == '\\' || regex_match(url, pattern))
			{
				fs::create_directories(url);
				return make_unique<LOCAL>(component_name, url, component_location);
			}
		}
		catch (...)
		{
			throw ClientError();
		}

		throw UnknownClient();
	}
}
