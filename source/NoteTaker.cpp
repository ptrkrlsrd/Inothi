/**
*    Notetaker.cpp
*    Purpose: Main operation of the app 
*    @author Petter Karlsrud 
*    @version 1.0 15/11/16 
**/
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
  vector<Note> noteArray;
  Configuration config = Configuration("./config.cfg");

  /* *
   * Get the path to the notes
   * @return Returns the path to the notes.
  * */
  string getNotePath() { return config.path.c_str(); }

  /* *
   * Create an vector of Notes from a vector of strings.
   * @param Takes a vector of strings.
   * @return Returns a vector of notes.
  * */
  vector<Note> parseVector(vector<string> lines) {
    vector<Note> localArray;
    for (string line : lines) {
      vector<string> lines = Utilities::split(line, '#');
      
      int index = stoi(lines[0]);
      ptime timeStamp = Utilities::timeFromString(lines[1]);
      string content = lines[2];
      Note note = Note(index,timeStamp,content);
      localArray.push_back(note);
    }
    return localArray;
  }

  /* *
   * Read the note file and update the noteArray
  * */
  vector<Note> fetchNotes() {
    vector<string> lines = InOut::readFile(getNotePath());
    return parseVector(lines);
  }

  /* *
   * Make a new note and append it to the note file.
  * */
  Note makeNote() {
    int lineCount = InOut::countLines(getNotePath());
    string noteContent;
    ptime now = boost::posix_time::microsec_clock::universal_time();

    Utilities::getUserInput("New Note", &noteContent);
    time_facet *facet = new time_facet();
    facet->format("%H:%M");
    stringstream stream;
    stream.imbue(locale(locale::classic(), facet));
    stream << now;
    cout << stream.str();

    string content = Utilities::capitalize(noteContent);
    Note note = Note(lineCount, now, content);
    return note;
  }

  void appendNoteToNotes(Note note, vector<Note> *inArray) {
    inArray->push_back(note);  // Add to the notearray
  }

  /* *
   * List all the notes in a vector of notes.
   * @param vector of notes
  * */
  void listNotes(vector<Note> *inArray) {
    cout << "\n";
    for (Note note : *inArray) {
      cout << note.toString() << "\n";
    }
  }

  /* *
   * Delete a note from a vector of notes by index.
   * @param vector of notes
   * @param index you want to remove
  * */
  void deleteNoteAtIndex(vector<Note> *inArray, int index) {
    inArray->erase(inArray->begin() + index);
  }

  /* *
   * Perform a delete operation
   * by asking the user which file to delete.
   * @param takes a vector of notes
  * */
  void deleteNote(vector<Note> *inArray) {
    string input;
    cout << "Delete note nr: \n";
    getline(cin, input);

    if (input == "all") {
      deleteAll(&noteArray);
    } else {
      int index = stoi(input);
      int lineCount = InOut::countLines(getNotePath());
      if (index < lineCount) {
        deleteNoteAtIndex(inArray, index);
      } else {
        int correctedCount = lineCount - 1;
        cout << "Out of range. (max " << correctedCount
             << ") Pick a different number.\n";
        deleteNote(&noteArray);
      }
    }
  }

  void deleteAll(vector<Note> *inArray) {
    inArray = new vector<Note>;
  }

  /* *
   * Save a vector of notes to a path.
   * @param vector of notes
   * @param path to the file
  * */
  void saveNotesToFile(vector<Note> *inArray, string path) {
    vector<string> linesToWrite;
    int index = 0;
    for (Note note : *inArray) {
      note.index = index;
      linesToWrite.push_back(note.toString());
      index++;
    }

    try {
      InOut::writeFile(config.path, linesToWrite);
    } catch (exception &e) {
      throw e;
    }
  }

  /* *
   * Handle the arguments
   * @param arguments
  * */
  void handleArguments(char *arguments[]) {
    string mode = arguments[1];
    if (mode == "list" || mode == "-l" || mode == "-ls" || mode == "--list") {
      listNotes(&noteArray);
    } else if (mode == "delete" || mode == "-rm") {
      deleteNote(&noteArray);
      listNotes(&noteArray);
      saveNotesToFile(&noteArray, config.path);
    } else if (mode == "--help") {
      cout << "Available commands: list(-l, --list), delete(-rm) & --help"
           << endl;
    } else {
      cout << "Invalid option: " << mode << "\n";
    }
  }

  /* *
   * Initializer for the NoteTaker.
   * Handles the flags and the configuration file.
   * @param count of arguments
   * @param arguments
  * */
  void init(int argCount, char *arguments[]) {
    noteArray = fetchNotes();
    /*If theres just one argument(i.e the command),
    then create a new note*/
    if (argCount == 1) {
      Note newNote = makeNote();
      appendNoteToNotes(newNote, &noteArray);

      InOut::appendLineToFile(getNotePath(),
      newNote.toString());  // Write to the file

      listNotes(&noteArray);
    } else {
      /*If there are more than one argument, 
      then handle the arguments*/
      handleArguments(arguments);
    }
  }

  /**
   * Constructor for the Notetaker 
   * @param count of arguments
   * @param arguments
  **/
  NoteTaker (int argCount, char *arguments[]) {
    init(argCount, arguments);
  }
};

int main(int argc, char *argv[]) {
  //Initialize the notetaker class
  NoteTaker noteTaker = NoteTaker(argc, argv);
  return 0;
}

