#pragma once
#include <fstream>
#include <iostream>
#include <vector>

namespace host {
  const std::vector<char> readFileAsChar(const std::string & filePath);
}