#include "Note.h"
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;
const string PATH = "/Users/petterkarlsrud/.ptr/";
const string FILENAME = "Note.txt";

string timeNow () {
  time_t _tm =time(NULL);
  struct tm * curtime = localtime(&_tm);
  char output[30];
  strftime(output, 30, "%Y %m-%d %a %H:%M", curtime);
  return string(output);
}

void appendLineToFile(string fileName, string line) {
  ofstream file;
  file.open(fileName, ios_base::app);
  string writeLine = timeNow() + ": " + line;
  file << writeLine << "\n";
  file.close();
}

void readFile(string fileName) {
  string line;
  ifstream file;
  file.open(fileName);
  while(getline(file, line)) {
    cout << line << "\n";
  }
}

int main() {
  string input;
  getline(cin, input);
  appendLineToFile(PATH + FILENAME, input);
  readFile(PATH + FILENAME);
  return 0;
}

