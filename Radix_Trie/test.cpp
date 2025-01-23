#include "Trie.hpp"
#include <assert.h>
#include <unordered_set>
#include <fstream>
#include <filesystem>

void basic_tests() {
    Trie trie;
    trie.insert("world");
    trie.insert("word");
    trie.insert("wordy");
    trie.insert("w");
    trie.insert("someword");
    assert(!trie.isEmpty());

    std::vector<std::string> comp;

    trie.getCompletions("", comp);

    // Test insert acontainsnd 
    assert(trie.contains("world") == true);
    std::cout << "Test passed: 'world' found in trie." << std::endl;

    assert(trie.contains("word") == true);
    std::cout << "Test passed: 'word' found in trie." << std::endl;

    assert(trie.contains("wordy") == true);
    std::cout << "Test passed: 'wordy' found in trie." << std::endl;

    assert(trie.contains("w") == true);
    std::cout << "Test passed: 'w' found in trie." << std::endl;

    // Test remove
    // trie.remove("w");
    // assert(trie.contains("w") == false);
    // std::cout << "Test passed: 'w' removed from trie." << std::endl;

    // Test getCompletions
    comp.clear();
    trie.getCompletions("wor", comp);
    assert(comp.size() == 3);
    std::cout << "Test passed: 3 completions found for prefix 'wor'." << std::endl;

    comp.clear();
    trie.getCompletions("wo", comp);
    assert(comp.size() == 3);
    std::cout << "Test passed: 3 completions found for prefix 'wo'." << std::endl;

    comp.clear();
    trie.getCompletions("word", comp);
    assert(comp.size() == 2);
    std::cout << "Test passed: 2 completions found for prefix 'word'." << std::endl;


    comp.clear();
    trie.getCompletions("", comp);
    trie.contains("w");
}


void test_trie_mem(const std::string &filepath) {
    std::filesystem::path p(filepath);
    assert(std::filesystem::exists(p));
    std::ifstream in_file;
    std::string line;
    in_file.open(filepath);
    // std::unordered_set<std::string>word_set;
    Trie trie;

    while (!in_file.eof()) {
        std::getline(in_file, line);
        trie.insert(line);
    }
    // while (true) {

    // } 
    std::cout << "Inserted " << trie.wordCount() << " words into trie \n";
}
int main() {
    Trie trie;
    trie.insert("a");
    trie.insert("apple");
    trie.insert("hello");
    trie.insert("zeitgeist");
    trie.insert("application");
    trie.insert("app");
    // std::vector<std::string>comp;
    std::vector<std::string> comp;
    if (trie.getCompletions("", comp)) {
        for (const std::string &s : comp) {
            std::cout << s << "\n";
        }
    }
    test_trie_mem("words_alpha.txt");
    
    
}