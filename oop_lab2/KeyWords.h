#pragma once
#include <string>
namespace labKeyWords
{
	const std::string DESC = "desc";
	const std::string CSED = "csed";
	const std::string OPERATOR_NEXT = "->";
	const std::string OPERATOR_SET = "=";

	const std::string READFILE = "readfile";
	const std::string WRITEFILE = "writefile";
	const std::string GREP = "grep";
	const std::string SORT = "sort";
	const std::string REPLACE = "replace";
	const std::string DUMP = "dump";
	const std::string EMPTY_BLOCK = "empty block"; // formally not a keyword

	bool isOperator(const std::string& str);

	bool isBlockType(const std::string& str);

	bool isKeyword(const std::string& str);
}