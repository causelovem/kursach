CXX=g++
CXXFLAGS= -Wall -g -O0

OBJS=$(patsubst %.cpp, %.o,  $(wildcard *.cpp))

all: PROGRAMM NAME

PROGRAMM NAME: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm $(OBJS) PROGRAMM NAME

