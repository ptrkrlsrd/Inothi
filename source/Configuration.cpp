#include "Configuration.h"
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

Configuration::Configuration (string filePath) {
  libconfig::Config cfgHandler;
  try {
    cfgHandler.readFile(filePath.c_str());
  } catch(const FileIOException &fioex) {
    cerr << "I/O error while reading file." << "\n";
  } catch(const ParseException &pex) {
    cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
      << " - " << pex.getError() << "\n";
  }

  try {
    string _path = cfgHandler.lookup("path");
    path = _path;
  } catch(const SettingNotFoundException &nfex) {
    cerr << "No 'path' setting in configuration file." << "\n";
  }
}
