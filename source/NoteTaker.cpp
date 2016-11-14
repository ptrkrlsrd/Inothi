#include "NoteTaker.h"
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <locale>
#include <string>
#include <vector>
#include <boost/date_time.hpp>
#include <libconfig.h++>
#include "Utilities.cpp"
#include "InOut.cpp"

using namespace std;
using namespace libconfig;
using namespace boost::gregorian;
using namespace boost::posix_time;

class NoteTaker {
  public:
    vector <Note> noteArray;
    string NotePath;
    
    string getNotePath () {
      return Utilities::getHomeDir() + "/.notes.txt";
    }

    void setup () {
      string selectedPath;
      cout << "Select where you want to save your notes(Absolute path): " << "\n";
      getline(cin, selectedPath);
      cout << "Does this look correct(y/n)? " << selectedPath << "\n";
      string answer;
      getline(cin, answer);
      if (answer == "y") {
        NotePath = selectedPath;
      } else if(answer == "n") {
        setup();
      } else {
        //Ask again 
      }
    }
    
    vector<Note> parseVector(vector<string> lines) {
      vector<Note> localArray;
      for (string line : lines) {
        vector<string> lines = Utilities::split(line, '#');
        Note note;
        note.index = stoi(lines[0]);
        note.timeStamp = Utilities::timeFromString(lines[1]);
        note.content = lines[2];
        localArray.push_back(note);
      }
      return localArray;
    }

    void updateArray () {
      vector <string> lines = InOut::readFile(getNotePath());
      noteArray = parseVector(lines);
    }

    void makeNote () {
      Note note;
      string input;
      int lineCount = InOut::countLines(getNotePath());
      ptime now = boost::posix_time::microsec_clock::universal_time();

      cout << "New note" << "\n";
      getline(cin, input);

      note.index = lineCount;
      note.timeStamp = now;
      note.content = input;
      
      noteArray.push_back(note);
      InOut::appendLineToFile(getNotePath(), note.toString());
    }

    void listNotes (vector<Note> *inArray) {
      cout << "\n";
      for (Note note : *inArray) {
        cout << note.index << " " << note.timeStamp.date() << " " << note.content << "\n";
      }
    }

    void deleteNoteAtIndex (vector<Note> *inArray, int index) {
      inArray -> erase(inArray -> begin() + index);
    }

    void deleteNote (vector<Note> *inArray) {
      string input;
      cout << "Delete note nr: " << "\n";
      getline(cin, input);
      int index = stoi(input);
      int lineCount = InOut::countLines(getNotePath());
      if (index < lineCount) {
        deleteNoteAtIndex(inArray, index);
      } else {
        cout << "Out of range. (max " << lineCount - 1 <<  ") Pick a different number." << "\n";
        deleteNote(&noteArray);
      }
    }

    void saveNotesToFile (vector<Note> *inArray, string path) {
      for (Note note : *inArray) {}
    }

    void readConfig () {
      Config cfg;
      try {
        cfg.readFile("notetaker.cfg");
      } catch(const FileIOException &fioex) {
        std::cerr << "I/O error while reading file." << "\n";
      } catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
          << " - " << pex.getError() << "\n";
      }

      try {
        string path = cfg.lookup("path");
      } catch(const SettingNotFoundException &nfex) {
        cerr << "No 'path' setting in configuration file." << "\n";
      }
    }

    void init (int argCount, char *arguments[]) {
      readConfig();

      if (argCount == 1) {
        updateArray();
        makeNote();
        listNotes(&noteArray);
      } else {
        string mode = arguments[1];
        updateArray();

        if (mode == "setup" || mode == "Setup") {
          setup();
        } else if (mode == "list" || mode == "-l" || mode == "--list") {
          listNotes(&noteArray);
        } else if (mode == "delete" || mode == "-rm") {
          deleteNote(&noteArray);
          listNotes(&noteArray);
        } else{
          cout << "Invalid option: " << mode << "\n";
        }
      }
    }
};

int main (int argc, char *argv[]) {
  NoteTaker noteTaker;
  noteTaker.init(argc, argv);
  return 0;
}

