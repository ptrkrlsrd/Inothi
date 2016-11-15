CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -lboost_date_time -lconfig++
BIN=NoteTaker

SRC=source/NoteTaker.cpp

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f *.o
	rm $(BIN)
