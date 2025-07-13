// GumboParser.cpp : GumboParser class definition for parsing HTML content using the Gumbo library
//
#include "GumboParser.hpp"

#include <gumbo.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

// Function to recursively extract text from a Gumbo node
std::string clean_text(GumboNode *node)
{
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    std::string text = "";
    for (unsigned int i = 0; i < node->v.element.children.length; ++i) {
      text += clean_text(static_cast<GumboNode *>(node->v.element.children.data[i]));
    }
    return text;
  }
  return "";
}

// Function to find elements by tag name
void find_elements_by_tag(GumboNode *node, GumboTag tag, std::vector<GumboNode *> &results)
{
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  if (node->v.element.tag == tag) {
    results.push_back(node);
  }
  for (unsigned int i = 0; i < node->v.element.children.length; ++i) {
    find_elements_by_tag(static_cast<GumboNode *>(node->v.element.children.data[i]), tag, results);
  }
}

// Helper to trim whitespace from a string
std::string trim(const std::string &str)
{
  size_t first = str.find_first_not_of(" \t\n\r");
  if (std::string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(" \t\n\r");
  return str.substr(first, (last - first + 1));
}

// Helper to find an attribute by name
const GumboAttribute *find_attribute(const GumboVector *attributes, const std::string &name)
{
  for (unsigned int i = 0; i < attributes->length; ++i) {
    GumboAttribute *attr = static_cast<GumboAttribute *>(attributes->data[i]);
    if (std::string(attr->name) == name) {
      return attr;
    }
  }
  return nullptr;
}

// Function to find a specific table by a set of required header texts
auto find_table_by_required_headers(GumboNode *root_node, const std::vector<std::string> &required_headers)
{
  std::vector<GumboNode *> tables;
  find_elements_by_tag(root_node, GUMBO_TAG_TABLE, tables);

  for (GumboNode *table_node : tables) {
    std::vector<GumboNode *> trs_in_table;
    find_elements_by_tag(table_node, GUMBO_TAG_TR, trs_in_table);

    if (trs_in_table.empty()) {
      continue;  // Skip tables with no rows
    }

    // Assume the first row contains headers
    GumboNode *header_row = trs_in_table[0];
    std::vector<GumboNode *> cells_in_header_row;
    // Check for both TD and TH, as headers might be in TD in some tables
    find_elements_by_tag(header_row, GUMBO_TAG_TD, cells_in_header_row);
    if (cells_in_header_row.empty()) {  // If no TDs, check for THs
      find_elements_by_tag(header_row, GUMBO_TAG_TH, cells_in_header_row);
    }

    std::set<std::string> found_headers_in_table;
    for (GumboNode *cell_node : cells_in_header_row) {
      std::string cell_text = trim(clean_text(cell_node));
      found_headers_in_table.insert(cell_text);
    }

    // Check if all required headers are present in the current table's headers
    bool all_headers_found = true;
    for (const std::string &required_header : required_headers) {
      if (found_headers_in_table.find(required_header) == found_headers_in_table.end()) {
        all_headers_found = false;
        break;
      }
    }

    if (all_headers_found) {
      return table_node;  // This is the table we're looking for
    }
  }
  throw std::exception("No table found with the required headers.");
}

void ParseHtml(const std::string &htmlContent)
{
  GumboOutput *output = gumbo_parse(htmlContent.c_str());

  std::vector<std::map<std::string, std::string>> tableData;
  std::vector<std::string> headers;

  // --- Modify this line to use the new function and list all required headers ---
  std::vector<std::string> required_headers = {"Општина", "Време", "Улице"};
  GumboNode *target_table = find_table_by_required_headers(output->root, required_headers);

  if (!target_table) {
    std::cerr << "Error: Target table not found with all specified headers!" << std::endl;
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return;
  }

  // --- 2. Extract Headers from the first row ---
  std::vector<GumboNode *> trs_in_table;
  find_elements_by_tag(target_table, GUMBO_TAG_TR, trs_in_table);

  if (trs_in_table.empty()) {
    std::cerr << "Error: No rows found in the target table!" << std::endl;
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return;
  }

  GumboNode *header_row = trs_in_table[0];
  std::vector<GumboNode *> header_cells;
  find_elements_by_tag(header_row, GUMBO_TAG_TD, header_cells);  // Headers are in <td> in your HTML

  for (GumboNode *cell : header_cells) {
    std::string header_text = trim(clean_text(cell));
    headers.push_back(header_text);
  }

  if (headers.empty()) {
    std::cerr << "Error: No headers found in the table!" << std::endl;
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return;
  }

  // --- 3. Extract Data Rows (starting from the second row) ---
  for (size_t i = 1; i < trs_in_table.size(); ++i) {  // Start from index 1 to skip header row
    GumboNode *data_row = trs_in_table[i];
    std::vector<GumboNode *> data_cells;
    find_elements_by_tag(data_row, GUMBO_TAG_TD, data_cells);

    std::map<std::string, std::string> row_map;
    for (size_t j = 0; j < data_cells.size() && j < headers.size(); ++j) {
      std::string cell_text = trim(clean_text(data_cells[j]));
      row_map[headers[j]] = cell_text;
    }
    tableData.push_back(row_map);
  }

  // --- 4. Print the extracted data to verify ---
  std::cout << "Extracted Table Data:" << std::endl;
  for (const auto &row : tableData) {
    for (const auto &pair : row) {
      std::cout << pair.first << ": " << pair.second << std::endl;
    }
    std::cout << "--------------------" << std::endl;
  }

  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return;
}
