#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <locale>

namespace codeHelper 
{
	// locale struct required to set locales in a stream to argument (DelimsLocale)
	// required to set delimiters for "operator >>" defined in ifstream
	struct DelimsLocale : std::ctype<char>
	{
		DelimsLocale(std::string const &delims) : std::ctype<char>(get_table(delims)) {}

		static std::ctype_base::mask const* get_table(std::string const &delims)
		{
			static std::vector<std::ctype_base::mask> delimiters(table_size, std::ctype_base::mask());
			for (unsigned char ch : delims)
				delimiters[ch] = std::ctype_base::space;

			return &delimiters[0];
		}
	};

	std::string fileNotOpened(const std::string& fileName);

	void cutEndLine(std::string& str);
}
