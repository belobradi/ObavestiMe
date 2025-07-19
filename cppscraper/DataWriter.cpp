// DataWriter.cpp : DataWriter definition for inserting parsed data into a database
//
#include "DataWriter.hpp"

#include <sqlite3.h>

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

static auto callback(void* data, int argc, char** argv, char** azColName)
{
  for (int i = 0; i < argc; i++) {
    std::cout << (azColName[i] ? azColName[i] : "NULL") << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
  }
  return 0;
}

fs::path GetUserDataDir()
{
  fs::path dataDir;
  const auto appName{"cppscraper"};

#ifdef _WIN32
  const auto appData{std::getenv("APPDATA")};
  if (appData) {
    dataDir = fs::path(appData) / appName;
  } else {
    dataDir = fs::temp_directory_path() / appName;
  }
#elif defined(__APPLE__)
  const auto{std::getenv("HOME")};
  if (home) {
    dataDir = fs::path(home) / "Library" / "Application Support" / appName;
  } else {
    dataDir = fs::temp_directory_path() / appName;
  }
#else
  const auto{std::getenv("HOME")};
  if (home) {
    dataDir = fs::path(home) / ".config" / appName;
  } else {
    dataDir = fs::temp_directory_path() / appName;
  }
#endif

  if (!fs::exists(dataDir)) {
    fs::create_directories(dataDir);
  }

  return dataDir;
}

auto InitializeDatabase(sqlite3** db)
{
  const auto dbFilePath{GetUserDataDir() / "scraped_outage_data.db"};
  const auto rc{sqlite3_open(dbFilePath.string().c_str(), db)};
  if (rc) {
    throw std::runtime_error{std::format("Can't open database at {}: {}", dbFilePath.string(), sqlite3_errmsg(*db))};
  }
  std::cout << "Opened database at: " << dbFilePath << std::endl;
}

auto CloseDatabase(sqlite3* db)
{
  sqlite3_close(db);
  std::cout << "Database closed" << std::endl;
}

auto CreateScrapedDataTable(sqlite3* db)
{
  const auto sqlCreate{
      "CREATE TABLE IF NOT EXISTS scrapped_outage_data (area INTEGER, dayOffset INTEGER, municipality TEXT, time TEXT, streets "
      "TEXT);"};
  char* errMsg = 0;
  const auto rc{sqlite3_exec(db, sqlCreate, callback, 0, &errMsg)};
  if (rc != SQLITE_OK) {
    const auto message{std::format("SQL error: {}", errMsg)};
    sqlite3_free(errMsg);
    CloseDatabase(db);
    throw std::runtime_error{message};
  }
  std::cout << "Table created successfully" << std::endl;
}

auto InsertScrapedData(sqlite3* db, const int area, const int dayOffset, const std::vector<std::vector<std::string>>& tableData)
{
  const auto sqlInsert{
      "INSERT INTO scrapped_outage_data (area, dayOffset, municipality, time, streets) VALUES (?, ?, ?, ? ,?)"};
  sqlite3_stmt* stmt;
  for (const auto& row : tableData) {
    if (row.size() != 3) {
      std::cerr << "Unexpected number of columns in table. Data is not written." << std::endl;
    }
    auto rc{sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, 0)};
    if (rc != SQLITE_OK) {
      std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_bind_int(stmt, 1, area);
    sqlite3_bind_int(stmt, 2, dayOffset);
    sqlite3_bind_text(stmt, 3, row[0].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, row[1].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, row[2].c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
      std::cerr << "SQL insert error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
  }
}

}  // namespace

void WriteData(const int area, const int dayOffset, const std::vector<std::vector<std::string>>& tableData)
{
  sqlite3* db{nullptr};
  InitializeDatabase(&db);
  CreateScrapedDataTable(db);
  InsertScrapedData(db, area, dayOffset, tableData);
  CloseDatabase(db);
}
