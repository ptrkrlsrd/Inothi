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
   * Counts the lines of a file.
   * @param the filename of the file.
   * @return line count.
   * */
  int countLines(string fileName) {
    ifstream inFile(fileName);
    int index = count(istreambuf_iterator<char>(inFile),
        istreambuf_iterator<char>(), '\n');
    return index;
  }

  /* *
   * Read a file and return the lines as a vector
   * @param the filename of the file.
   * @return the lines as a vector of strings.
   * */
  vector<string> readFile(string fileName) {
    vector<string> localArray;
    string line;
    ifstream file;
    file.open(fileName);

    try {
      while(getline(file, line)) {
        localArray.push_back(line);
      }
    } catch (const ifstream::failure& e){
      throw e;
    }
    return localArray;
  }

  /* *
   * Append a line to a file
   * @param the filename of the file.
   * */
  void appendLineToFile(string fileName, string line) {
    ofstream file;
    file.open(fileName, ios_base::app);
    try {
      file << line << "\n";
      file.close();
    } catch (const ifstream::failure& e){
      throw e;
    }
  }

  /* *
   * Write a file by overwriting.
   * @param the filename of the file.
   * @param the lines you want to write.
   * */
  void writeFile(string fileName, vector<string> lines) {
    ofstream file;
    file.open(fileName, ios_base::trunc);

    try {
      for (string line : lines) {
        file << line << "\n";
      }
      file.close();
    } catch (const ifstream::failure& e){
      throw e;
    }
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
};
