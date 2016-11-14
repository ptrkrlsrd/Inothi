#include "Note.h"
#include "Utilities.cpp"
#include "InOut.cpp"
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <locale>
#include <string>
#include <vector>
#include <boost/date_time.hpp>

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

class NoteTaker {
  public:
    vector <NoteElement> noteArray;

    vector<NoteElement> parseVector(vector<string> lines) {
      vector<NoteElement> localArray;
      for (string line : lines) {
        vector<string> lines = Utilities::split(line, '#');
        NoteElement note;
        note.index = stoi(lines[0]);
        note.timeStamp = Utilities::timeFromString(lines[1]);
        note.content = lines[2];
        localArray.push_back(note);
      }
      return localArray;
    }

    void updateArray () {
      vector <string> lines = InOut::readFile(PATH + FILENAME);
      noteArray = parseVector(lines);
    }

    void setup () {
      string selectedPath;
      cout << "Select where you want to save your notes(Absolute path): " << "\n";
      getline(cin, selectedPath);
      cout << "Path: " << selectedPath << "\n";
    }

    void makeNote () {
      NoteElement element;
      string input;
      string fileName = PATH + FILENAME;
      ptime now = boost::posix_time::microsec_clock::universal_time();

      cout << "New note" << "\n";
      getline(cin, input);

      int lineCount = InOut::countLines(fileName);

      element.index = lineCount;
      element.timeStamp = now;
      element.content = input;

      InOut::appendLineToFile(fileName, element.toString());
    }

    void listNotes (vector<NoteElement> inArray) {
      for(NoteElement note : inArray) {
        cout << note.index << " " << note.timeStamp.date() << " " << note.content << "\n";
      }
    }

    vector<NoteElement>  deleteNoteElementAtIndex (vector<NoteElement> inArray, int index) {
      vector<NoteElement> localArray(inArray);
      localArray.erase(localArray.begin() + index);
      return localArray;
    }

    vector<NoteElement>  deleteNoteElement (vector<NoteElement> inArray) {
      string input;
      cout << "Delete note nr: " << "\n";
      getline(cin, input);
      int index = stoi(input);
      vector<NoteElement> localArray = deleteNoteElementAtIndex(inArray, index);
      return localArray;
    }

    void performDelete (vector<NoteElement> inArray) {
      noteArray = deleteNoteElement(noteArray);
      listNotes(noteArray);
    }

    void saveNotesToFile (vector<NoteElement> inArray, string path) {
    }

    void init (int argCount, char *arguments[]){
      if (argCount == 1) {
        updateArray();
        makeNote();
        listNotes(noteArray);
      } else {
        string mode = arguments[1];
        updateArray();

        if (mode == "setup" || mode == "Setup") {
          setup();
        } else if (mode == "list" || mode == "-l" || mode == "--list") {
          listNotes(noteArray);
        } else if (mode == "delete" || mode == "-rm") {
          performDelete(noteArray);
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

