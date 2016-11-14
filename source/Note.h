#include <vector>
#include <boost/date_time.hpp>

using namespace std;
using namespace boost::posix_time;
class NoteElement {
  public:
    int index;
    string content;
    ptime timeStamp;
    
    string toString() {
      return to_string(index) + '#' + to_simple_string(timeStamp) + '#' + content;
    }
};
