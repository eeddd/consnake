CXX = g++
CXXFLAGS = -Wall -g
LIBS = -lncurses

main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o $(LIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm main *.o