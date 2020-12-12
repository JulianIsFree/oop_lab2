#ifndef HELPER_H
#define HELPER_H
#include <vector>
#include <string>

namespace labHelper
{
	void createFile(const std::string &fileName, const std::vector<std::string>& content);
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
}

#endif // !HELPER_H