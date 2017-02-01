#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

const char TIME_FORMAT[] = "%Y %m-%d %a %H:%M";

namespace Utilities {
  string dateToString (time_t _tm) {
    struct tm * curtime = localtime(&_tm);
    char output[30];
    strftime(output, 30, TIME_FORMAT, curtime);
    return string(output);
  }

  boost::posix_time::ptime timeFromString (string &inString) {
    return time_from_string(inString);
  }

  void split (const string &inString, char delimiter, vector<string> &elements) {
    stringstream splitString;
    splitString.str(inString);
    string item;
    while (getline(splitString, item, delimiter)) {
      elements.push_back(item);
    }
  }

  vector<string> split (const string &inString, char delimiter) {
    vector<string> elements;
    split(inString, delimiter, elements);
    return elements;
  }

  string capitalize (string inString) {
    inString[0] = toupper(inString[0]);
    return inString;
  }

  string getHomeDir () {
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
      homedir = getpwuid(getuid())->pw_dir;
    }
    return homedir;
  }

  string getUserInput (string question) {
    string input = "";
    cout << question << "\n";
    getline(cin, input);
    return input;
  }

  void getUserInput (string question, string *answer) {
    cout << question << endl;
    getline(cin, *answer);
  }
};
