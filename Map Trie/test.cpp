#include "Trie.hpp"
#include <fstream>
#include <unordered_set>
#include <filesystem>


void test_trie_from_txt(const std::string &filepath) {
    std::filesystem::path p(filepath);
    assert(std::filesystem::exists(p));
    std::ifstream in_file;
    std::string line;
    in_file.open(filepath);
    std::unordered_set<std::string>word_set;
    Trie trie;

    while (!in_file.eof()) {
        std::getline(in_file, line);
        //The trie expects only lowercase strings for now 
        // this workaround just makes sure the set works fine
        for (char &c : line) {
            c = std::tolower(c); 
        }
        word_set.emplace(line) ;
        trie.insert(line);
    }
    std::vector<std::string>failed_matches;
    int matched_cnt = 0;
    for (const std::string &s : word_set) {
        bool res = trie.search(s);
        if (!res) {
            std::cout << "Failed at string " << res << "\n";
            failed_matches.push_back(s);
            continue;
        }
        matched_cnt++;
    }
    if (!failed_matches.empty()) {
        for (const auto &s : failed_matches) {
            std::cout << "trie failed to insert/search string: " << s << "\n";
        }
    }
    std::cout << matched_cnt << "\n";
    assert(matched_cnt == word_set.size());
    std::cout << "passed test for 1000 insertions and 1000 trie.search()\n";
    std::vector<std::string>comp;
    trie.getCompletions("", comp);
    std::cout << "Completions from empty string " << comp.size() << "\n";
    std::cout << "size of word_set:  " << word_set.size() << "\n";

    for (const std::string &s : comp) {
        auto res = word_set.find(s);
        if (res == word_set.end()) {
            std::cout << "Failed to match completion list to string " << s << " \n";
        }
    }
    std::cout << "passed test for finding all words in completion inside word_set \n";

    for (const std::string &s : word_set) {
        trie.remove(s);
    }
    for (const std::string &s : word_set) {
        assert(trie.search(s) == false);
    }

    std::vector<std::string>comp2; // should be empty
    bool res = trie.getCompletions("", comp2);
    assert(comp2.size() == 0);
    for (const auto &s : comp2) {
        std::cout << "Failed to remove ? :" <<  s << "\n";
    }
    std::cout << "succesfully removed all strings in word set\n";
}

void basic_tests() {
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

int main() {

    // https://github.com/powerlanguage/word-lists/blob/master/1000-most-common-words.txt
    test_trie_from_txt("1000-most-common-words.txt");

    // https://github.com/dwyl/english-words/blob/master/words_alpha.txt
    test_trie_from_txt("words_alpha.txt");

}