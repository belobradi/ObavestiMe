// main.cpp : Defines the entry point for the application.
//
#include <iostream>

#include "Common.hpp"
#include "GumboParser.hpp"
#include "HtmlReader.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  for (auto area{0}; area < area::COUNT; ++area) {
    for (auto dayOffset{0}; dayOffset < dayoffset::COUNT; ++dayOffset) {
      try {
        const auto htmlContent{ReadHtml(area, dayOffset)};
        const auto tableData{ExtractTable(htmlContent)};
        // InsertTableData(tableData);
      } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
      } catch (...) {
        std::cerr << "Unknown exception occurred." << '\n';
      }
    }
  }
  return 0;
}
