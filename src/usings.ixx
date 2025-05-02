module;

#include <map>
#include <string>
#include <vector>

export module usings;

using namespace std;

/// @brief Zkratka pro konstantní referenci.
/// @tparam T - Libovolný typ, u kterého dává smysl mít konstantní reference.
export template <class T>
using cr = const T&;

/// @brief Seznam souborů členěný podle kategorií.
export using file_map = map<string, vector<string>>;
