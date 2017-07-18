#include "Configuration.h"
#include "Utilities.cpp"
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

/* *
 * Initializer for the Configuration. 
 * @param the filename of the configuration file.
 * @return the configuration itself.
 * */
Configuration::Configuration (string filePath) {
  libconfig::Config cfgHandler;

  //Set the default path to $HOME/.notes
  path = Utilities::getHomeDir() + "/.notes.txt"; 

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

Configuration::Configuration () {
  libconfig::Config cfgHandler;

  //Set the default path to $HOME/.notes
  path = Utilities::getHomeDir() + "/.notes.txt"; 

  try {
    cfgHandler.readFile("./config.cfg");
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
