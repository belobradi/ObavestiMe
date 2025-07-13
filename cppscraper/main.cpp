// main.cpp : Defines the entry point for the application.
//
#include <iostream>

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
  try {
    const auto htmlContent{ReadHtml()};
    ParseHtml(htmlContent);
    // WriteTableData();

    // std::cout << htmlContent << "\n";
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Unknown exception occurred." << '\n';
    return 1;
  }
}
