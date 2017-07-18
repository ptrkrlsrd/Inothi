/**
*    InOut.cpp
*    Purpose: Perform In/Out operations
*    @author Petter Karlsrud
*    @version 1.0 15/11/16
**/

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"

using namespace std;

namespace InOut {
  /* *
   * Check if a file exists
   * @param the filename of the file.
   * @return bool
   * */
  bool checkIfFileExists(string fileName) {
    ifstream infile(fileName);
    return infile.good();
  }

  /* *
   * Read the json file specified in the path
   * @param the filename of the file.
   * */
  nlohmann::json readJsonFile(string path) {
    nlohmann::json jsonStruct;
    std::ifstream jsonFile(path);
    jsonFile >> jsonStruct;
    return jsonStruct;
  }

  /* *
    * Creates a new json fil at the specified path
    * @param the filename of the file.
    * */
  void createNewJsonFile (string path) {
    nlohmann::json jsonStruct = nlohmann::json::object();
    jsonStruct["notes"] = nlohmann::json::array();
    std::ofstream o(path);
    o << std::setw(4) << jsonStruct << std::endl;
  }
};
