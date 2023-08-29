#include <iostream>
#include <fstream>
#include <stdexcept>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
public:
    LogMessage(Type type, const std::string& message) : messageType(type), messageContent(message) {}

    Type type() const {
        return messageType;
    }

    const std::string& message() const {
        return messageContent;
    }

private:
    Type messageType;
    std::string messageContent;
};

class LogHandler {
public:
    LogHandler(LogHandler* nextHandler = nullptr) : next(nextHandler) {}

    virtual void handle(const LogMessage& msg) = 0;

    void setNext(LogHandler* handler) {
        next = handler;
    }

protected:
    LogHandler* next;
};

class FatalErrorHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::FatalError) {
            throw std::runtime_error(msg.message());
        }

        if (next) {
            next->handle(msg);
        }
    }
};

class FileErrorHandler : public LogHandler {
public:
    FileErrorHandler(const std::string& filePath) : errorFilePath(filePath) {}

    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::Error) {
            std::ofstream fileStream(errorFilePath, std::ios::app);
            if (fileStream.is_open()) {
                fileStream << "Error: " << msg.message() << std::endl;
            }
        }

        if (next) {
            next->handle(msg);
        }
    }

private:
    std::string errorFilePath;
};

class ConsoleWarningHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::Warning) {
            std::cout << "Warning: " << msg.message() << std::endl;
        }

        if (next) {
            next->handle(msg);
        }
    }
};

class UnknownMessageHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::Unknown) {
            throw std::runtime_error("Unhandled message type: " + msg.message());
        }

        if (next) {
            next->handle(msg);
        }
    }
};

int main() {
    FatalErrorHandler fatalErrorHandler;
    FileErrorHandler fileErrorHandler("error.log");
    ConsoleWarningHandler consoleWarningHandler;
    UnknownMessageHandler unknownMessageHandler;

    fatalErrorHandler.setNext(&fileErrorHandler);
    fileErrorHandler.setNext(&consoleWarningHandler);
    consoleWarningHandler.setNext(&unknownMessageHandler);

    LogMessage warningMessage(Type::Warning, "This is a warning.");
    LogMessage errorMessage(Type::Error, "This is an error.");
    LogMessage fatalErrorMessage(Type::FatalError, "This is a fatal error.");
    LogMessage unknownMessage(Type::Unknown, "This is an unknown message.");

    try {
        fatalErrorHandler.handle(warningMessage);
        fatalErrorHandler.handle(errorMessage);
        fatalErrorHandler.handle(fatalErrorMessage);
        fatalErrorHandler.handle(unknownMessage);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
