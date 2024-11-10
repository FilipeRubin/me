#include "console-logger.h"
#include <iostream>

void ConsoleLogger::LogInfo(const std::string& content)
{
	PrintMessage("Info", content);
}

void ConsoleLogger::LogWarning(const std::string& content)
{
	PrintMessage("Warning", content);
}

void ConsoleLogger::LogError(const std::string& content)
{
	PrintMessage("Error", content);
}

void ConsoleLogger::PrintMessage(const std::string& type, const std::string& content) const
{
	std::cout << '[' << type << "]: " << content << '\n';
}
