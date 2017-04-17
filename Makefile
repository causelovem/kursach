GREEDY_FILE = greedy
GEN_FILE = com_matrix_gen
CXX = g++
CXXFLAGS = -g -O3

all: $(GEN_FILE) $(GREEDY_FILE)

map:
	./$(GREEDY_FILE) matrix 27

gen:
	./$(GEN_FILE) matrix 27

$(GREEDY_FILE): $(GREEDY_FILE).cpp
	$(CXX) $(CXXFLAGS) $(GREEDY_FILE).cpp -o $(GREEDY_FILE)

$(GEN_FILE): $(GEN_FILE).cpp
	$(CXX) $(CXXFLAGS) $(GEN_FILE).cpp -o $(GEN_FILE)

clean:
	rm -rf $(GEN_FILE)
	rm -rf $(GREEDY_FILE)
	rm -rf *.o