#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << "Console Log: " << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
public:
    FileLogCommand(const std::string& filePath) : filePath(filePath) {}

    void print(const std::string& message) override {
        std::ofstream fileStream(filePath, std::ios::app);
        if (fileStream.is_open()) {
            fileStream << "File Log: " << message << std::endl;
        }
    }

private:
    std::string filePath;
};

void executeLogCommand(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    ConsoleLogCommand consoleLog;
    FileLogCommand fileLog("log.txt");

    std::string message = "message";

    executeLogCommand(consoleLog, message);
    executeLogCommand(fileLog, message);

    return 0;
}
