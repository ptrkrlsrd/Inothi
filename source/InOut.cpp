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

using namespace std;

namespace InOut {
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
};
