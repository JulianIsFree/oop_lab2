#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace labException
{
	class Exception : std::exception
	{
		std::string message;
	public:
		Exception(const std::string& message) : message(message) {};
		std::string what() { return message; };
	};

	class FileNotOpened : Exception
	{
	public:
		FileNotOpened(const std::string& fileName) : Exception("Can't open file " + fileName) {};
	};

	class FileFailError : Exception
	{
	public:
		FileFailError(const std::string& fileName) : Exception("Error occured with file " + fileName) {};
	};

	class LexicalError : Exception
	{
	public:
		LexicalError(const std::string& word) : Exception("Word " + word + " contains lexical error") {};
	};

	class BadBlockException : Exception
	{
	public:
		BadBlockException(const std::string& why) : Exception("You don't want to get bad block: " + why) {};
	};
}

#endif // !EXCEPTION_H