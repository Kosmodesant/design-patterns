#include <iostream>
#include <string>
#include <vector>

class SqlSelectQueryBuilder {
private:
    std::string query;

public:
    SqlSelectQueryBuilder() {
        query = "SELECT * ";
    }

    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        if (query.find("*") != std::string::npos) {
            // Replace * with the first column name
            size_t pos = query.find("*");
            query.replace(pos, 1, column);
        }
        else {
            query.insert(query.length() - 1, ", " + column);
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        size_t from_pos = query.find("FROM ");
        size_t where_pos = query.find("WHERE");

        if (from_pos != std::string::npos) {
            if (where_pos != std::string::npos) {
                query.replace(from_pos + 5, where_pos - from_pos - 6, table);
            }
            else {
                query.replace(from_pos + 5, query.length() - from_pos - 6, table);
            }
        }
        else {
            query += "FROM " + table;
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        if (query.find("WHERE") == std::string::npos) {
            query += " WHERE ";
        }
        else {
            query += " AND ";
        }
        query += column + "=" + value;
        return *this;
    }

    std::string BuildQuery() const {
        return query + ";";
    }
};

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::string expected_query = "SELECT name, phone FROM students WHERE id=42 AND name=John;";

    if (query_builder.BuildQuery() == expected_query) {
        std::cout << "Query is correct!" << std::endl;
    }
    else {
        std::cout << "Query is incorrect!" << std::endl;
    }

    return 0;
}
