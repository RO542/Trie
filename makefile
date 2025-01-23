CXX := clang
FLAGS := -O3 -std=c++17 #cpp 17 for std::filesystem

ROOTDIR := $(realpath .)
MAP_TRIE_DIR  := $(ROOTDIR)/Map_Trie/
ARRAY_TRIE_DIR := $(ROOTDIR)/Array_Trie/
RADIX_TRIE_DIR  := $(ROOTDIR)/Radix_Trie/

# -g for debug informtation
test_all: build_all
	${RADIX_TRIE_DIR}tests.exe
	${MAP_TRIE_DIR}tests.exe
	# ${ARRAY_TRIE_DIR}tests.exe

build_all: map_trie radix_trie #array_trie

map_trie: $(MAP_TRIE_DIR)Trie.cpp trie_tests.cpp
	$(CXX) $(FLAGS) -I$(MAP_TRIE_DIR) $(MAP_TRIE_DIR)Trie.cpp $(ROOTDIR)/trie_tests.cpp -o $(MAP_TRIE_DIR)tests.exe

array_trie: $(ARRAY_TRIE_DIR)Trie.cpp trie_tests.cpp
	$(CXX) $(FLAGS) -I$(ARRAY_TRIE_DIR) $(ARRAY_TRIE_DIR)Trie.cpp $(ROOTDIR)/trie_tests.cpp -o $(ARRAY_TRIE_DIR)tests.exe

radix_trie: $(RADIX_TRIE_DIR)Trie.cpp trie_tests.cpp
	$(CXX) $(FLAGS) -I$(RADIX_TRIE_DIR) $(RADIX_TRIE_DIR)Trie.cpp $(ROOTDIR)/trie_tests.cpp -o $(RADIX_TRIE_DIR)tests.exe

