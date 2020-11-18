#include "KeyWords.h"

bool labKeyWords::isOperator(const std::string & str)
{
	if (str == labKeyWords::OPERATOR_NEXT)
		return true;

	if (str == labKeyWords::OPERATOR_SET)
		return true;

	return false;
}

bool labKeyWords::isBlockType(const std::string& str)
{
	if (str == labKeyWords::READFILE)
		return true;

	if (str == labKeyWords::WRITEFILE)
		return true;

	if (str == labKeyWords::GREP)
		return true;

	if (str == labKeyWords::SORT)
		return true;

	if (str == labKeyWords::REPLACE)
		return true;

	if (str == labKeyWords::DUMP)
		return true;

	return false;
}

bool labKeyWords::isKeyword(const std::string& str)
{
	if (str == labKeyWords::DESC)
		return true;

	if (str == labKeyWords::CSED)
		return true;

	return labKeyWords::isBlockType(str) || labKeyWords::isOperator(str);
}