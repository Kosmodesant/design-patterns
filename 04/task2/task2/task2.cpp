#include <iostream>
#include <fstream>
#include <stdexcept>

// Интерфейс для форматирования данных
class Formatter
{
public:
    virtual ~Formatter() = default;
    virtual std::string format(const std::string& data) const = 0;
};

// Конкретный класс форматтера для HTML
class HTMLFormatter : public Formatter
{
public:
    std::string format(const std::string& data) const override
    {
        return "<html>" + data + "</html>";
    }
};

// Конкретный класс форматтера для текста
class TextFormatter : public Formatter
{
public:
    std::string format(const std::string& data) const override
    {
        return data;
    }
};

// Конкретный класс форматтера для JSON
class JSONFormatter : public Formatter
{
public:
    std::string format(const std::string& data) const override
    {
        return "{ \"data\": \"" + data + "\"}";
    }
};

// Интерфейс для печати данных в различных форматах
class Printable
{
public:
    virtual ~Printable() = default;
    virtual std::string getData() const = 0;
    virtual const Formatter& getFormatter() const = 0;
};

// Класс Data для хранения данных
class Data : public Printable
{
public:
    Data(std::string data, const Formatter& formatter)
        : data_(std::move(data)), formatter_(formatter) {}

    std::string getData() const override
    {
        return data_;
    }

    const Formatter& getFormatter() const override
    {
        return formatter_;
    }

private:
    std::string data_;
    const Formatter& formatter_;
};

// Функция для сохранения данных в файл
void saveTo(std::ofstream& file, const Printable& printable)
{
    file << printable.getFormatter().format(printable.getData());
}

int main()
{
    HTMLFormatter htmlFormatter;
    TextFormatter textFormatter;
    JSONFormatter jsonFormatter;

    Data htmlData("HTML Data", htmlFormatter);
    Data textData("Text Data", textFormatter);
    Data jsonData("JSON Data", jsonFormatter);

    std::ofstream htmlFile("output.html");
    saveTo(htmlFile, htmlData);
    htmlFile.close();

    std::ofstream textFile("output.txt");
    saveTo(textFile, textData);
    textFile.close();

    std::ofstream jsonFile("output.json");
    saveTo(jsonFile, jsonData);
    jsonFile.close();

    return 0;
}
