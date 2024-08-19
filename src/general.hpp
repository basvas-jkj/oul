#pragma once

#include <string>
#include <vector>

namespace oul
{
	/**
	 * @brief Rozd�l� �et�zec na pod�et�zce podle zadan�ho odd�lova�e.
	 * @param s rozd�lovan� �et�zec
	 * @param ch odd�lova�
	 * @return seznam pod�et�zc�
	 **/
	std::vector<std::string> split(const std::string& s, char ch);
}
