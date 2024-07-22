#include "general.hpp"

using namespace std;

namespace oul
{
	vector<string> split(string s, char ch)
	{
		vector<string> v;
		string::iterator start = s.begin();

		while (start != s.end())
		{
			string::iterator end = find(start, s.end(), ch);
			v.push_back(string(start, end));

			if (end == s.end())
			{
				break;
			}
			else
			{
				start = end + 1;
			}
		}

		return v;
	}
}