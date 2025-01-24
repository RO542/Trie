#include <fstream>
#include <unordered_set>
#include <filesystem>

#include "Trie.hpp"

void test_bulk_insert(const std::string &filepath = "words_alpha.txt") {
    std::cout << "Inserting all words from file: " << filepath << "\n";
    std::filesystem::path p(filepath);
    assert(std::filesystem::exists(p));
    std::ifstream in_file;
    std::string line;
    in_file.open(filepath);

    Trie trie;
    while (!in_file.eof()) {
        std::getline(in_file, line);
        trie.insert(line);
    }
    in_file.close();
    std::vector<std::string>completions;
    trie.getCompletions("h", completions);
    for (const auto &word: completions) {
        std::cout << word << "\n";
    }
    

    std::cout << "Inserted " << trie.wordCount() << " Words";
}



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

    // Test insert and search
    assert(trie.contains("world") == true);
    std::cout << "Test passed: 'world' found in trie." << std::endl;

    assert(trie.contains("word") == true);
    std::cout << "Test passed: 'word' found in trie." << std::endl;

    assert(trie.contains("wordy") == true);
    std::cout << "Test passed: 'wordy' found in trie." << std::endl;

    assert(trie.contains("w") == true);
    std::cout << "Test passed: 'w' found in trie." << std::endl;

    // Test remove
    trie.remove("w");
    assert(trie.contains("w") == false);
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
    trie.contains("w");
}

void trie_test1() {
    Trie trie;

    // Test insert and search
    trie.insert("hello");
    assert(trie.contains("hello") == true);
    std::cout << "Test passed: 'hello' found in trie." << std::endl;

    trie.insert("hell");
    assert(trie.contains("hell") == true);
    std::cout << "Test passed: 'hell' found in trie." << std::endl;

    trie.insert("heaven");
    assert(trie.contains("heaven") == true);
    std::cout << "Test passed: 'heaven' found in trie." << std::endl;

    trie.insert("goodbye");
    assert(trie.contains("goodbye") == true);
    std::cout << "Test passed: 'goodbye' found in trie." << std::endl;

    // Test remove
    trie.remove("hello");
    assert(trie.contains("hello") == false);
    std::cout << "Test passed: 'hello' removed from trie." << std::endl;

    // Test getCompletions
    std::vector<std::string> completions;
    trie.getCompletions("he", completions);
    assert(completions.size() == 2);
    std::cout << "Test passed: 2 completions found for prefix 'he'." << std::endl;

    completions.clear();
    trie.getCompletions("go", completions);
    assert(completions.size() == 1);
    std::cout << "Test passed: 1 completion found for prefix 'go'." << std::endl;

    completions.clear();
    trie.getCompletions("h", completions);
    assert(completions.size() == 2);
    std::cout << "Test passed: 2 completions found for prefix 'h'." << std::endl;

    completions.clear();
    trie.getCompletions("", completions);
    assert(completions.size() == 3);
    std::cout << "Test passed: 3 completions found for empty prefix." << std::endl;

    // Test isEmpty and wordCount
    assert(trie.isEmpty() == false);
    std::cout << "Test passed: Trie is not empty." << std::endl;

    assert(trie.wordCount() == 3);
    std::cout << "Test passed: Trie word count is 3." << std::endl;

    trie.clear();
    assert(trie.isEmpty() == true);
    std::cout << "Test passed: Trie is empty after clear." << std::endl;

    assert(trie.wordCount() == 0);
    std::cout << "Test passed: Trie word count is 0 after clear." << std::endl;
}

int main() {
    test_bulk_insert();

    // Trie trie;
    // trie.insert("hello");
    // trie.insert("heat");
    // trie.insert("hellope");
    // trie.insert("world");
    // // std::vector<std::string> comp;
    // std::cout << trie.contains("hello") << "\n";
    // trie.remove("hello");
    // std::cout << trie.contains("hello") << "\n";
    
    // trie.clear();

}