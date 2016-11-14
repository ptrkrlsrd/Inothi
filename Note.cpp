#include "Note.h"
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <ctime>
#include <iostream>
#include <locale>
#include <string>
#include <sstream>
#include <vector>
#include <boost/date_time.hpp>

using namespace std;
using namespace boost::gregorian;

const string PATH = "/Users/petterkarlsrud/.ptr/";
const string FILENAME = "Note.txt";
const char TIME_FORMAT[] = "%Y %m-%d %a %H:%M";

string dateToString (time_t _tm) {
  struct tm * curtime = localtime(&_tm);
  char output[30];
  strftime(output, 30, TIME_FORMAT, curtime);
  return string(output);
}

time_t timeFromString(string inString) {
  std::locale::global(std::locale("ja_JP.utf8"));
  std::time_t t = std::time(NULL);
  char mbstr[100];
  return strftime(mbstr, sizeof(mbstr), "%A %c", inString);
}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void appendLineToFile(string fileName, string line) {
  ofstream file;
  file.open(fileName, ios_base::app);
  std::ifstream inFile(fileName);
  int index = std::count(std::istreambuf_iterator<char>(inFile), 
      std::istreambuf_iterator<char>(), '\n');

  try {
    NoteElement element;
    time_t tm = time(NULL);
    string content = to_string(index) + '#' + dateToString(tm) + '#' + line;

    element.content = content;
    file << content << "\n";
    file.close();
  } catch (const ifstream::failure& e){
    throw e;
  }
}

void readFile(string fileName) {
  string line;
  ifstream file;
  file.open(fileName);

  try {
    while(getline(file, line)) {
      std::vector<std::string> x = split(line, '#');
      NoteElement note;
      note.index = std::stoi( x[0] );
      note.timeStamp = timeFromString(x[1]);
      note.content = x[2];
      cout << note.index << " " << x[1] << " " << note.content << "\n";
    }
  } catch (const ifstream::failure& e){
    throw e;
  }
}

void setup () {
  string selectedPath;
  cout << "Select where you want to save your notes(Absolute path): " << "\n";
  getline(cin, selectedPath);
  cout << "Path: " << selectedPath << "\n";
}

void makeNote (bool displayOutput) {
  string input;
  cout << "New note" << "\n";
  getline(cin, input);

  appendLineToFile(PATH + FILENAME, input);
  if (displayOutput) {
    readFile(PATH + FILENAME);
  }
}

void listNotes () {
  readFile(PATH + FILENAME);
}

void deleteNote () {

}

int main (int argc, char *argv[]) {
  if (argc == 1) {
    makeNote(true);
  } else {
    string argument = argv[1];

    if (argument == "setup" || argument == "Setup") {
      setup();
    } else if (argument == "list" || argument == "-l" || argument == "--list") {
      listNotes();
    } else if (argument == "delete" || argument == "-rm") {
      deleteNote();
    } else {
      cout << "Invalid option: " << argument << "\n";
    }
  }

  return 0;
}
