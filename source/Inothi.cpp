/**
 *    Inothi.cpp
 *    Purpose: Main operation of the app
 *    @author Petter Karlsrud
 *    @version 1.0 15/11/1
 **/
#include <boost/date_time.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <locale>
#include <string>
#include <vector>
#include "Note.h"
#include "Configuration.cpp"
#include "InOut.cpp"
#include "json.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

class Inothi {
  public:
    vector<Note> noteArray;
    Configuration config;

    /* *
     * Constructor for the Notetaker
     * @param count of arguments
     * @param arguments
     * */
    Inothi (int argCount, char *arguments[]) {
      init(argCount, arguments);
    }

    /* *
     * Initializer for the Inothi.
     * Handles the flags and the configuration file.
     * @param count of arguments
     * @param arguments
     * */
    void init(int argCount, char *arguments[]) {
      config = Configuration("./config.cfg");
      if (!InOut::checkIfFileExists(config.path)) {
        InOut::createNewJsonFile(config.path);
      }
      noteArray = readNotesFromFile(config.path);
    }

    /* *
     * Get the path to the notes
     * @return Returns the path to the notes.
     * */
    string getNotePath(Configuration *inConfig) { return inConfig->path.c_str(); }

    /* *
     * Read the note file and update the noteArray
     * @return Returns a vector of notes.
     * */
    vector<Note> readNotesFromFile(string path) {
      vector<Note> outNotes;

      std::ifstream inFile(path);
      nlohmann::json fileJson;
      inFile >> fileJson;
      vector<nlohmann::json> notes = fileJson["notes"];

      for (nlohmann::json i : notes) {
        Note note;
        string timeStampString = i["date"];
        ptime timeStamp = Utilities::timeFromString(timeStampString);
        note.index = i["index"];
        vector<string> a = i["tags"];
        note.tags = a;
        note.timeStamp = timeStamp;
        note.content = i["content"];
        outNotes.push_back(note);
      }
      return outNotes;
    }

    /* *
     * Make a new note and append it to the note file.
     * @return Returns a new Note
     * */
    Note newNote() {
      int lineCount = countNotes();
      ptime now = boost::posix_time::microsec_clock::universal_time();
      string noteContent = Utilities::getUserInput("New Note");
      time_facet *facet = new time_facet();
      stringstream stream;

      facet->format("%H:%M");
      stream.imbue(locale(locale::classic(), facet));
      stream << now;

      string content = Utilities::capitalize(noteContent);
      Note note = Note(lineCount, now, content);

      string input = Utilities::getUserInput("Do you want to add tags? <y/n>\n");
      if (input == "y" || input == "yes") {
        note.tags = createTags();
      }

      return note;
    }

    /* *
     * Create new tags
     * @return a vector of string
     * */
    vector<string> createTags() {
      string tags = Utilities::getUserInput("Please enter your tags.\n(Ex: tag, tag, another tag)\n");
      boost::erase_all(tags, " ");
      vector<string> splitTags = Utilities::split(tags, ',');
      return splitTags;
    }

    void appendNoteToNotes(Note note) {
      noteArray.push_back(note);  // Add to the notearray
    }

    /* *
     * List all the notes in a vector of notes.
     * */
    void listNotes() {
      cout << "\n";
      for (Note note : noteArray) {
        cout  << note.toDetailedString() << "\n\n";
      }
    }

    /* *
     * Count the notes in a vector of notes.
     * */
    int countNotes() {
      return noteArray.size();
    }

    /* *
     * Delete a note from a vector of notes by index.
     * @param index you want to remove
     * */
    void deleteNoteAtIndex(int index) {
      noteArray.erase(noteArray.begin() + index);
    }

    /* *
     * Perform a delete operation
     * by asking the user which file to delete.
     * */
    void deleteNote() {
      string input;
      listNotes();
      cout << "Delete note nr: \n";
      getline(cin, input);

      if (input == "all") {
        deleteAll();
      } else {
        int index = stoi(input);
        string notePath = getNotePath(&config);
        int lineCount = countNotes();

        if (index < lineCount) {
          deleteNoteAtIndex(index);
        } else {
          int correctedCount = lineCount - 1;
          cout << "Out of range. (max " << correctedCount
            << ") Pick a different number.\n";
          deleteNote();
        }
      }
    }

    /* *
     * Delete all item in an array
     * @param vector of notes
     * */
    void deleteAll() {
      noteArray.clear();
      saveNotesToFile();
    }

    /* *
     * Save a vector of notes to a path.
     * */
    void saveNotesToFile() {
      nlohmann::json jsonFile = InOut::readJsonFile(config.path);
      vector<nlohmann::json> jsonNotesArray;

      for (Note i : noteArray) {
        nlohmann::json object = nlohmann::json::object();
        object["index"] = i.index;
        object["date"] = to_simple_string(i.timeStamp);
        object["tags"] = i.tags;
        object["content"] = i.content;
        jsonNotesArray.push_back(object);
      }

      std::ofstream o(config.path);
      jsonFile["notes"] = jsonNotesArray;
      o << std::setw(4) << jsonFile << std::endl;
    }
};

int main(int argc, char *argv[]) {
  //Initialize the notetaker class
  Inothi instance = Inothi(argc, argv);

  if (argc == 1) {
    /*If theres just one argument(i.e the command),
      then create a new note*/
    Note note = instance.newNote();
    instance.appendNoteToNotes(note);
    instance.saveNotesToFile();
    instance.listNotes();
  } else {
    /*If there are more than one argument,
      then handle the arguments*/
    string mode = argv[1];

    if (mode == "list" || mode == "-l" || mode == "-ls" || mode == "--list") {
      instance.listNotes();
    } else if (mode == "delete" || mode == "-rm") {
      instance.deleteNote();
      instance.listNotes();
      instance.saveNotesToFile();
    } else if (mode == "--clear") {
      instance.deleteAll();
    } else if (mode == "--help") {
      cout << "Available commands: list(-l, --list), delete(-rm) & --help\n";
    } else {
      cout << "Invalid option: " << mode << "\n";
    }
  }
  return 0;
}

