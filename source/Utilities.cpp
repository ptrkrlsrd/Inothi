#include <ctime>
#include <string>
#include <sstream>
#include <vector>

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

  ptime timeFromString(string inString) {
    return time_from_string(inString);
  }

  void split(const string &inString, char delimiter, vector<string> &elements) {
    stringstream splitString;
    splitString.str(inString);
    string item;
    while (getline(splitString, item, delimiter)) {
      elements.push_back(item);
    }
  }

  vector<string> split(const string &inString, char delimiter) {
    vector<string> elements;
    split(inString, delimiter, elements);
    return elements;
  }
};