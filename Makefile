CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -lboost_date_time -lconfig++
BIN=Inothi
SRC=source/Inothi.cpp

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

clean:
	rm $(BIN)

