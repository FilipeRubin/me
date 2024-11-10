#pragma once
#include "i-logger.h"

class ConsoleLogger : public ILogger
{
public:
	void LogInfo(const std::string& content) override;
	void LogWarning(const std::string& content) override;
	void LogError(const std::string& content) override;
private:
	void PrintMessage(const std::string& type, const std::string& content) const;
};
