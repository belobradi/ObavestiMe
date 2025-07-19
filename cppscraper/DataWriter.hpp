// DataWriter.hpp : DataWriter definition for inserting parsed data into a database
//
#pragma once

#include <string>
#include <vector>

void WriteData(const int area, const int dayOffset, const std::vector<std::vector<std::string>>& tableData);
