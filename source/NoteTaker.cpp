#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <locale>
#include <string>
#include <vector>
#include <boost/date_time.hpp>
#include "Note.h"
#include "Utilities.cpp"
#include "Configuration.cpp"
#include "InOut.cpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

class NoteTaker {
  public:
    vector <Note> noteArray;
    Configuration config = Configuration("./config.cfg");

    /* *
     * Get the path to the notes 
     * @return Returns the path to the notes. 
    * */
    string getNotePath () {
      return config.path.c_str();
    }

    /* *
     * Create an vector of Notes from a vector of strings. 
     * @param Takes a vector of strings. 
     * @return Returns a vector of notes.
    * */
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

    /* *
     * Read the note file and update the noteArray 
    * */
    void updateArray () {
      vector <string> lines = InOut::readFile(getNotePath());
      noteArray = parseVector(lines);
    }

    /* *
     * Make a new note and append it to the note file. 
    * */
    void makeNote () {
      int lineCount = InOut::countLines(getNotePath());
      string input;
      ptime now = boost::posix_time::microsec_clock::universal_time();

      Utilities::getUserInput("New Note", &input);
      time_facet* facet = new time_facet();
      facet->format("%H:%M");
      stringstream stream;
      stream.imbue(locale(locale::classic(), facet));
      stream << now;
      cout << stream.str(); 

      Note note;
      note.timeStamp = now;
      note.index = lineCount;
      note.content = Utilities::capitalize(input);
      appendNoteToNotes(note);
    }

    void appendNoteToNotes(Note note) {
      noteArray.push_back(note); //Add to the notearray
      InOut::appendLineToFile(getNotePath(), note.toString()); //Write to the file
    }

    /* *
     * List all the notes in a vector of notes. 
     * @param Takes a vector of notes. 
    * */
    void listNotes (vector<Note> *inArray) {
      cout << "\n";
      for (Note note : *inArray) {
        cout << note.index << " " << note.timeStamp.date() << " " << note.content << "\n";
      }
    }

    /* *
     * Delete a note from a vector of notes by index.
     * @param Takes a vector of notes. 
     * @param The index you want to remove. 
    * */
    void deleteNoteAtIndex (vector<Note> *inArray, int index) {
      inArray -> erase(inArray -> begin() + index);
    }

    /* *
     * Perform a delete operation 
     * by asking the user which file to delete.
     * @param Takes a vector of notes. 
    * */
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
      saveNotesToFile(inArray, config.path);
    }

    /* *
     * Save a vector of notes to a path. 
     * @param Takes a vector of notes. 
     * @param The path to the file. 
    * */
    void saveNotesToFile (vector<Note> *inArray, string path) {
      vector <string> linesToWrite;
      int index = 0;
      for (Note note : *inArray) {
        note.index = index;
        linesToWrite.push_back(note.toString());
        index++;
      }

      try {
        InOut::writeFile(config.path, linesToWrite);
      } catch(exception &e) {
        throw e;
      }
    }

    /* *
     * Initializer for the NoteTaker.
     * Handles the flags and the configuration file.
     * @param The count of arguments.
     * @param The arguments. 
    * */
    void init (int argCount, char *arguments[]) {
      if (argCount == 1) {
        updateArray();
        makeNote();
        listNotes(&noteArray);
      } else {
        string mode = arguments[1];
        updateArray();

        if (mode == "list" || mode == "-l" || mode == "-ls" || mode == "--list") {
          listNotes(&noteArray);
        } else if (mode == "delete" || mode == "-rm") {
          deleteNote(&noteArray);
          listNotes(&noteArray);
        } else if (mode == "--help") {
          cout << "Available commands: list(-l, --list), delete(-rm) & --help"
            << endl;
        } else {
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

