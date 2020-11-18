#include "Helper.h"

std::string codeHelper::fileNotOpened(const std::string & fileName)
{
	return "file " + fileName + " isn't open";
}

void codeHelper::cutEndLine(std::string & str)
{
	const size_t len = str.length();
	if (str[len - 1] == '\n')
		str.pop_back();
}
