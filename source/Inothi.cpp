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
#include "Utilities.cpp"
#include "Configuration.cpp"
#include "InOut.cpp"
#include "json.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

class NoteTaker {
  public:
    vector<Note> noteArray;
    Configuration config;

    /* *
     * Constructor for the Notetaker
     * @param count of arguments
     * @param arguments
     * */
    NoteTaker (int argCount, char *arguments[]) {
      init(argCount, arguments);
    }

    /* *
     * Initializer for the NoteTaker.
     * Handles the flags and the configuration file.
     * @param count of arguments
     * @param arguments
     * */
    void init(int argCount, char *arguments[]) {
      config = Configuration("./config.cfg");
      if (!InOut::checkIfFileExists(config.path)) {
        createNewJsonFile(config.path);
      }
      noteArray = readNotesFromFile(config.path);

      /*If theres just one argument(i.e the command),
        then create a new note*/
      if (argCount == 1) {
        Note note = newNote();
        appendNoteToNotes(note, &noteArray);
        saveNotesToFile(&noteArray, config.path);
        listNotes(&noteArray);
      } else {
        /*If there are more than one argument,
          then handle the arguments*/
        handleArguments(arguments);
      }
    }

    /* *
     * Creates a new json fil at the specified path
     * @param the filename of the file.
     * */
    void createNewJsonFile (string path) {
      nlohmann::json jsonStruct = nlohmann::json::object();
      jsonStruct["notes"] = nlohmann::json::array();
      std::ofstream o(path);
      o << std::setw(4) << jsonStruct << std::endl;
    }

    /* *
     * Handle the arguments
     * @param arguments
     * */
    void handleArguments(char *arguments[]) {
      string mode = arguments[1];

      if (mode == "list" || mode == "-l" || mode == "-ls" || mode == "--list") {
        listNotes(&noteArray);
      } else if (mode == "-t") {
        Note note = newNote();
        appendNoteToNotes(note, &noteArray);
        InOut::appendLineToFile(getNotePath(&config),
            note.toStorableString());  // Write to the file
        listNotes(&noteArray);
      } else if (mode == "delete" || mode == "-rm") {
        deleteNote(&noteArray);
        listNotes(&noteArray);
        saveNotesToFile(&noteArray, config.path);
      } else if (mode == "--clear") {
        deleteAll(&noteArray);
        saveNotesToFile(&noteArray, config.path);
      } else if (mode == "--help") {
        cout << "Available commands: list(-l, --list), delete(-rm) & --help\n";
      } else {
        cout << "Invalid option: " << mode << "\n";
      }
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
      int lineCount = InOut::countLines(getNotePath(&config));
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
        cout  << note.toDetailedString() << "\n\n";
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
      listNotes(inArray);
      cout << "Delete note nr: \n";
      getline(cin, input);

      if (input == "all") {
        deleteAll(&noteArray);
      } else {
        int index = stoi(input);
        int lineCount = InOut::countLines(getNotePath(&config));
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

    /* *
     * Delete all item in an array
     * @param vector of notes
     * */
    void deleteAll(vector<Note> *inArray) {
      inArray -> clear();
    }

    /* *
     * Save a vector of notes to a path.
     * @param vector of notes
     * @param path to the file
     * */
    void saveNotesToFile(vector<Note> *inArray, string path) {
      nlohmann::json jsonFile = InOut::readJsonFile(path);
      vector<nlohmann::json> jsonNotesArray;

      for (Note i : noteArray) {
        nlohmann::json object = nlohmann::json::object();
        object["index"] = i.index;
        object["date"] = to_simple_string(i.timeStamp);
        object["tags"] = i.tags;
        object["content"] = i.content;
        jsonNotesArray.push_back(object);
      }

      std::ofstream o(path);
      jsonFile["notes"] = jsonNotesArray;
      o << std::setw(4) << jsonFile << std::endl;
    }
};

int main(int argc, char *argv[]) {
  //Initialize the notetaker class
  NoteTaker noteTaker = NoteTaker(argc, argv);
  return 0;
}

