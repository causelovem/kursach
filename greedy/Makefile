GREEDY_FILE = src/greedy
COM_MATRIX_GEN = src/com_matrix_gen
CHECK_PROG = src/prog_test
BUILD_MATRIX = src/build_matrix
BUILD_RAND_SEND = src/build_rand_send
CXX = g++
CXXFLAGS = -g -O3 -Wall

all: $(COM_MATRIX_GEN) $(GREEDY_FILE) $(BUILD_MATRIX) $(BUILD_RAND_SEND) $(CHECK_PROG)
 
map:
	./bin/greedy matrix512 512

gen:
	./bin/com_matrix_gen matrix512 512

check:
	rm -rf trace_folder/proc_*
	./bin/build_rand_send mas 512
	mpirun -np 512 ./bin/prog_test '/home/causelove/mishenka/prog/kursach/greedy/trace_folder' mas
	./bin/build_matrix matrix 512 trace_folder/*
	./bin/greedy matrix 512 mmm
	#cat mmm

$(GREEDY_FILE): $(GREEDY_FILE).cpp
	$(CXX) $(CXXFLAGS) $(GREEDY_FILE).cpp -o bin/greedy

$(COM_MATRIX_GEN): $(COM_MATRIX_GEN).cpp
	$(CXX) $(CXXFLAGS) $(COM_MATRIX_GEN).cpp -o bin/com_matrix_gen

$(BUILD_MATRIX): $(BUILD_MATRIX).cpp
	$(CXX) $(CXXFLAGS) $(BUILD_MATRIX).cpp -o bin/build_matrix

$(BUILD_RAND_SEND): $(BUILD_RAND_SEND).cpp
	$(CXX) $(CXXFLAGS) $(BUILD_RAND_SEND).cpp -o bin/build_rand_send

$(CHECK_PROG): $(CHECK_PROG).cpp
	mpicxx $(CXXFLAGS) -std=c++11 $(CHECK_PROG).cpp -o bin/prog_test

clean:
	rm -rf bin/*
	rm -rf *.o