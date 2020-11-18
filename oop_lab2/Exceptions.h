#pragma once
#include <iostream>
#include <string>

namespace labExceptions
{
	class Exception
	{
	protected:
		std::string message;
		Exception() : message("Exception") {};
	public:
		Exception(const std::string& message) { this->message = "Exception: " + message; };
		void printMessage() { std::cout << message << std::endl; };
		const std::string& getMessage() { return message; };
	};

	// Not expected token or expected token not found
	class BadToken : Exception
	{
	public:
		BadToken(const std::string& message) { this->message = "BadToken: " + message; };
	};

	// No such token found
	class CSEDNotFound : Exception
	{
	public:
		CSEDNotFound(const std::string &message) { this->message = "CSEDNotFound: " + message; };
	};

	// For every input error: from FileIsNotOpen error up to FailError
	class FileInputError : Exception
	{
	public:
		FileInputError(const std::string& message) { this->message = "FileInputError: " + message; };
	};

	// In parser: if some block's name used to assign new block
	class BlockRedefiniton : Exception
	{
	public:
		BlockRedefiniton(const std::string& blockID) { this->message = "Block already defined: " + blockID; };
	};

	class ParserStateError : Exception
	{
	public:
		ParserStateError(const std::string& message) { this->message = "Parser error: " + message; };
	};

	class BlockNotFound : Exception
	{
	public:
		BlockNotFound(const std::string& blockID) { this->message = "Can't find block with name: " + blockID; }
	};

	class BadBlock :Exception
	{
	public:
		BadBlock(const std::string& blockType) { this->message = "BadBlockType: " + blockType; };
	};
}