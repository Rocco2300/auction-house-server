#pragma once

#include "User.h"
#include "Entity.h"

#include <fmt/core.h>
#include <sqlite3.h>

inline bool IsDefault(const Field& field) {
    Field def(field.type());
    return (field == def);
}

inline std::string GetFormatWhereClause(const Field& field) {
    switch (field.type()) {
    case FieldType::Integer:
    case FieldType::Floating:
        return fmt::format(" {} = {}", field.name(), field);
    case FieldType::String:
        return fmt::format(" {} = '{}'", field.name(), field);
    }
    return fmt::format(" {} = '{}'", field.name(), field);
}

inline void PrepareStmt(sqlite3* db, sqlite3_stmt** stmt, Entity& entity) {
    std::string query =
            fmt::format("SELECT * FROM {} WHERE", entity.tableName());

    bool inserted = false;
    for (size_t i = 0; i < entity.size(); i++) {
        if (IsDefault(entity[i])) {
            continue;
        }

        if (inserted) {
            query += " AND";
        }

        query += GetFormatWhereClause(entity[i]);
        inserted = true;
    }
    query += ";";

    std::cout << query << '\n';

    int ret = sqlite3_prepare_v2(db, query.c_str(), query.length(), stmt, 0);
    if (ret) {
        std::cerr << "Prepare failed\n";
        return;
    }
}

inline Entity* StepStmtEntity(sqlite3_stmt* stmt) {
    int ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW) {
        return nullptr;
    }

    User* user = new User();
    for (int i = 0; i < user->size(); i++) {
        user->at(i) = sqlite3_column_blob(stmt, i);
    }

    return user;
}

inline void ReadDbEntity(sqlite3* db, Entity& entity) {
    sqlite3_stmt* stmt;
    PrepareStmt(db, &stmt, entity);

    auto returnEnt = StepStmtEntity(stmt);
    if (returnEnt) {
        entity = *returnEnt;
    }
}