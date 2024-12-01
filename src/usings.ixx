export module usings;

import <utility>;
using namespace std;

export namespace oul
{
	template <class T>
	using cr = const T&;

	template <class T>
	using option = pair<bool, T>;
}