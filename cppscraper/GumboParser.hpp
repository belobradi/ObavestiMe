// GumboParser.hpp : GumboParser definition for parsing HTML content using the Gumbo library
//
#pragma once

#include <string>
#include <vector>

std::vector<std::vector<std::string>> ExtractTable(const std::string &htmlContent);
