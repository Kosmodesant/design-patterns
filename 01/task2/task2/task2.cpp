#include <iostream>
#include <string>
#include <vector>
#include <map>

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

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) {
        if (!columns.empty()) {
            query.replace(query.find("*"), 1, columns.front());
            for (size_t i = 1; i < columns.size(); ++i) {
                query.insert(query.length() - 1, ", " + columns[i]);
            }
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        size_t from_pos = query.find("FROM ");
        size_t where_pos = query.find("WHERE");

        if (from_pos != std::string::npos) {
            if (where_pos != std::string::npos) {
                query.replace(from_pos + 5, where_pos - from_pos - 5, table);
            }
            else {
                query.replace(from_pos + 5, query.length() - from_pos - 5, table);
            }
        }
        else {
            query += " FROM " + table;
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

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        if (!kv.empty()) {
            if (query.find("WHERE") == std::string::npos) {
                query += " WHERE ";
            }
            else {
                query += " AND ";
            }

            bool first = true;
            for (const auto& entry : kv) {
                if (!first) {
                    query += " AND ";
                }
                query += entry.first + "=" + entry.second;
                first = false;
            }
        }
        return *this;
    }

    std::string BuildQuery() const {
        return query + ";";
    }
};

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumns({ "name", "phone" });
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::map<std::string, std::string> conditions = {
        {"id", "42"},
        {"name", "John"}
    };
    query_builder.AddWhere(conditions);

    std::string expected_query = "SELECT name, phone FROM students WHERE id=42 AND name=John;";

    if (query_builder.BuildQuery() == expected_query) {
        std::cout << "Query is correct!" << std::endl;
    }
    else {
        std::cout << "Query is incorrect!" << std::endl;
        std::cout << "Expected: " << expected_query << std::endl;
        std::cout << "Actual: " << query_builder.BuildQuery() << std::endl;
    }

    return 0;
}
