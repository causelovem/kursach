GREEDY_FILE = greedy
GEN_FILE = com_matrix_gen
CXX = g++
CXXFLAGS = -g -O3

all: $(GEN_FILE) $(GREEDY_FILE)

map:
	./$(GREEDY_FILE) matrix512 512

gen:
	./$(GEN_FILE) matrix512 512

$(GREEDY_FILE): $(GREEDY_FILE).cpp Makefile
	$(CXX) $(CXXFLAGS) $(GREEDY_FILE).cpp -o $(GREEDY_FILE)

$(GEN_FILE): $(GEN_FILE).cpp Makefile
	$(CXX) $(CXXFLAGS) $(GEN_FILE).cpp -o $(GEN_FILE)

clean:
	rm -rf $(GEN_FILE)
	rm -rf $(GREEDY_FILE)
	rm -rf *.o