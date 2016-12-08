#include <string>
#include <vector>
#include <boost/date_time.hpp>

using namespace std;
using namespace boost::posix_time;

class Note {
  public:
    int index;
    string content;
    ptime timeStamp;
    vector<string> tags;
    
    string toString() {
      return to_string(index) + '#' + to_simple_string(timeStamp) + '#' + content;
    }

    Note ();
    Note (int, ptime, string); 
};
Note::Note (){};
Note::Note (int _index, ptime _timeStamp, string _content)  {
  index = _index;
  timeStamp = _timeStamp;
  content = _content;
}
