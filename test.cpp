#include "Trie.hpp"


int main() {

    Trie trie;
    trie.insert("world");
    trie.insert("word");
    trie.insert("wordy");
    trie.insert("w");
    trie.insert("someword");



    std::cout << std::endl;
    std::vector<std::string> comp;


    trie.getCompletions("", comp);

    // Test insert and search
    assert(trie.search("world") == true);
    std::cout << "Test passed: 'world' found in trie." << std::endl;

    assert(trie.search("word") == true);
    std::cout << "Test passed: 'word' found in trie." << std::endl;

    assert(trie.search("wordy") == true);
    std::cout << "Test passed: 'wordy' found in trie." << std::endl;

    assert(trie.search("w") == true);
    std::cout << "Test passed: 'w' found in trie." << std::endl;

    // Test remove
    trie.remove("w");
    assert(trie.search("w") == false);
    std::cout << "Test passed: 'w' removed from trie." << std::endl;

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
    trie.search("w");


}