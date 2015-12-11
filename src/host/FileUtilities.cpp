#include "host/FileUtilities.hpp"

namespace host {

  const std::vector<char> readFileAsChar(const std::string & filePath) {
    std::ifstream file;

    file.open(filePath);

    if(!file.is_open()) {
      throw new std::runtime_error("The file cannot be read.");
    }

    file.seekg(0, std::ios::end);
    const auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> data(fileSize, 0);

    file.read(&data[0], fileSize);

    return data;
  }

}