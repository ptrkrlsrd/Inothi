#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
#define GOOGLE_ARRAYSIZE(a) \
    ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

namespace InOut {
  int countLines(string fileName) {
    ifstream inFile(fileName);
    int index = count(istreambuf_iterator<char>(inFile),
        istreambuf_iterator<char>(), '\n');
    return index;
  }

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
