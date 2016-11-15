# NoteTaker
A simple notetaker written in C++

## Usage

- Clone this repo
- Create a config file in the **root** and call it **config.cfg**
- Add this line:
`path = "</desired/path/to/notes.txt>";`

- Compile
`clang++ -std=c++11 -stdlib=libc++  -lboost_date_time -lconfig++ source/NoteTaker.cpp -o NoteTaker`
