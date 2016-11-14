#include <iostream>
#include <fstream>
#include <ctime>
#include <boost/date_time.hpp>

using namespace std;

class NoteElement {
  public:
    int index;
    string content;
    boost::gregorian::date timeStamp;
};

enum Mode { Setup, MakeNote, ListNotes, DeleteNote};

static void appendLineToFile(string fileName, string line);
static void readFile(string fileName);
void setup ();
void makeNote (bool displayOutput);
void listNotes ();
void deleteNote ();
string timeNow();
