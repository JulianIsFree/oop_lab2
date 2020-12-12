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

	class UnfinishedBlockException : Exception
	{
	public:
		UnfinishedBlockException(const std::string& why) : Exception("You don't want to get bad block: " + why) {};
	};

	class BlockDoesNotExistsException : Exception
	{
	public:
		BlockDoesNotExistsException(const std::string& name) : Exception("No such block with name: " + name) {};
	};

	class BlockRedefenitionException : Exception
	{
	public:
		BlockRedefenitionException(const std::string &name) : Exception("Redefinition of blocks isn't allowed: " + name) {};
	};

	class BlockSequenceIsNotAllowedException : Exception
	{
	public:
		BlockSequenceIsNotAllowedException(const std::string first, const std::string second) : 
			Exception("BlockType " + second + " isn't allowed after " + first) {};
	};

	class BadTokenException : Exception
	{
	public:
		BadTokenException(const std::string& token) : Exception("Token isn't allowed: " + token) {};
	};

	class ShouldNotReachThereException : Exception
	{
	public:
		ShouldNotReachThereException(const std::string& where) : Exception(where) {};
	};

	class UnexpectedEndOfFileException : Exception
	{
	public:
		UnexpectedEndOfFileException(const std::string& why) : Exception("Unexpected end of file: " + why) {};
	};

	class BlockNotFoundException : Exception
	{
	public:
		BlockNotFoundException(const std::string name) : Exception("Can't find block with name " + name) {};
	};
}

#endif // !EXCEPTION_H