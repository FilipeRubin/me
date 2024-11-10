#pragma once
#include <string>

class ILogger
{
public:
	virtual ~ILogger() = default;
	virtual void LogInfo(const std::string& content) = 0;
	virtual void LogWarning(const std::string& content) = 0;
	virtual void LogError(const std::string& content) = 0;
};
