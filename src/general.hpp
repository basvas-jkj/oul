#pragma once

#include <string>
#include <vector>

namespace oul
{
	/**
	 * @brief Rozdìlí øetìzec na podøetìzce podle zadaného oddìlovaèe.
	 * @param s rozdìlovaný øetìzec
	 * @param ch oddìlovaè
	 * @return seznam podøetìzcù
	 **/
	std::vector<std::string> split(const std::string& s, char ch);
}
