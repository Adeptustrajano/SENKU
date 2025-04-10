CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: mainGSenku

mainGSenku: mainGSenku.o GSenku.o
	$(CXX) $(CXXFLAGS) -o mainGSenku mainGSenku.o GSenku.o

mainGSenku.o: mainGSenku.cpp GSenku.hpp
	$(CXX) $(CXXFLAGS) -c mainGSenku.cpp

GSenku.o: GSenku.cpp GSenku.hpp
	$(CXX) $(CXXFLAGS) -c GSenku.cpp

clean:
	rm -f *.o mainGSenku