// GumboParser.cpp : GumboParser definition for parsing HTML content using the Gumbo library
//
#include "GumboParser.hpp"

#include <gumbo.h>

#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace {

auto CleanText(GumboNode *node)
{
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    std::string text = "";
    for (auto i = 0; i < node->v.element.children.length; ++i) {
      text += CleanText(static_cast<GumboNode *>(node->v.element.children.data[i]));
    }
    return text;
  }
  return std::string{};
}

auto FindElementsByTag(GumboNode *node, GumboTag tag, std::vector<GumboNode *> &results)
{
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  if (node->v.element.tag == tag) {
    results.push_back(node);
  }
  for (auto i = 0; i < node->v.element.children.length; ++i) {
    FindElementsByTag(static_cast<GumboNode *>(node->v.element.children.data[i]), tag, results);
  }
}

auto TrimString(const std::string &str)
{
  auto first{str.find_first_not_of(" \t\n\r")};
  if (std::string::npos == first) {
    return str;
  }
  auto last{str.find_last_not_of(" \t\n\r")};
  return str.substr(first, (last - first + 1));
}

auto GetTableHeaders(std::vector<GumboNode *> &tableRows)
{
  std::set<std::string> tableHeaders;
  std::vector<GumboNode *> columnCells;
  FindElementsByTag(tableRows[0], GUMBO_TAG_TD, columnCells);
  for (auto columnCell : columnCells) {
    auto cellText{TrimString(CleanText(columnCell))};
    tableHeaders.insert(cellText);
  }
  return tableHeaders;
}

auto FindTableByRequiredHeaders(GumboNode *rootNode, const std::vector<std::string> &requiredHeaders)
{
  std::vector<GumboNode *> tables;
  FindElementsByTag(rootNode, GUMBO_TAG_TABLE, tables);

  for (auto table : tables) {
    std::vector<GumboNode *> tableRows;
    FindElementsByTag(table, GUMBO_TAG_TR, tableRows);

    if (tableRows.empty() == true) {
      continue;
    }

    auto tableHeaders{GetTableHeaders(tableRows)};
    auto allHeadersFound{true};
    for (const auto &header : requiredHeaders) {
      if (tableHeaders.find(header) == tableHeaders.end()) {
        allHeadersFound = false;
        break;
      }
    }

    if (allHeadersFound == true) {
      return table;
    }
  }
  throw std::exception("No table found with the required headers.");
}

}  // namespace

std::vector<std::vector<std::string>> ExtractTable(const std::string &htmlContent)
{
  GumboOutput *output{gumbo_parse(htmlContent.c_str())};

  std::vector<std::string> requiredHeaders = {"Општина", "Време", "Улице"};

  std::vector<std::vector<std::string>> tableData;
  std::vector<GumboNode *> tableRows;
  try {
    auto table{FindTableByRequiredHeaders(output->root, requiredHeaders)};
    FindElementsByTag(table, GUMBO_TAG_TR, tableRows);
    for (auto i{1}; i < tableRows.size(); ++i) {
      auto tableRow{tableRows[i]};
      std::vector<GumboNode *> tableColumnCells;
      FindElementsByTag(tableRow, GUMBO_TAG_TD, tableColumnCells);

      std::vector<std::string> rowData;
      for (auto j{0}; j < tableColumnCells.size(); ++j) {
        rowData.push_back(TrimString(CleanText(tableColumnCells[j])));
      }
      tableData.push_back(rowData);
    }

    std::cout << "Extracted Table Data:" << std::endl;
    for (const auto &row : tableData) {
      for (const auto &col : row) {
        std::cout << col << std::endl;
      }
      std::cout << "--------------------" << std::endl;
    }
  } catch (const std::exception &e) {
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    throw std::exception{e.what()};
  }
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return tableData;
}
