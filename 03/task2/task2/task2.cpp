#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Observer {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class ConsoleWarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};

class FileErrorObserver : public Observer {
public:
    FileErrorObserver(const std::string& filePath) : filePath(filePath) {}

    void onError(const std::string& message) override {
        std::ofstream fileStream(filePath, std::ios::app);
        if (fileStream.is_open()) {
            fileStream << "Error: " << message << std::endl;
        }
    }

private:
    std::string filePath;
};

class ConsoleAndFileFatalErrorObserver : public Observer {
public:
    ConsoleAndFileFatalErrorObserver(const std::string& filePath) : filePath(filePath) {}

    void onFatalError(const std::string& message) override {
        std::cout << "Fatal Error: " << message << std::endl;

        std::ofstream fileStream(filePath, std::ios::app);
        if (fileStream.is_open()) {
            fileStream << "Fatal Error: " << message << std::endl;
        }
    }

private:
    std::string filePath;
};

class Observable {
public:
    void addWarningObserver(Observer* observer) {
        warningObservers.push_back(observer);
    }

    void addErrorObserver(Observer* observer) {
        errorObservers.push_back(observer);
    }

    void addFatalErrorObserver(Observer* observer) {
        fatalErrorObservers.push_back(observer);
    }

    void warning(const std::string& message) const {
        for (Observer* observer : warningObservers) {
            observer->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (Observer* observer : errorObservers) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (Observer* observer : fatalErrorObservers) {
            observer->onFatalError(message);
        }
    }

private:
    std::vector<Observer*> warningObservers;
    std::vector<Observer*> errorObservers;
    std::vector<Observer*> fatalErrorObservers;
};

int main() {
    Observable observable;

    ConsoleWarningObserver consoleWarningObserver;
    FileErrorObserver fileErrorObserver("error.log");
    ConsoleAndFileFatalErrorObserver consoleAndFileFatalErrorObserver("fatal_error.log");

    observable.addWarningObserver(&consoleWarningObserver);
    observable.addErrorObserver(&fileErrorObserver);
    observable.addFatalErrorObserver(&consoleAndFileFatalErrorObserver);

    observable.warning("This is a warning.");
    observable.error("This is an error.");
    observable.fatalError("This is a fatal error.");

    return 0;
}
