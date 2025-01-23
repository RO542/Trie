#include <unordered_map>
#include <stack>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

class Trie {
public:
    Trie();
    Trie(const std::vector<std::string> &string_list);
    ~Trie();
    size_t wordCount() const; // for testing 
    size_t nodeCount() const; // for testing 
    bool contains(const std::string &word) const;
    bool getCompletions(const std::string &str, std::vector<std::string> &out_completions) const;
    void insert(const std::string &word);

    void remove(const std::string &prefix);
    void clear();
    bool isEmpty() const;

    struct TrieNode {
        bool ends_word = false; // most nodes don't terminate words
        std::unordered_map<char, TrieNode *> children; //TODO: int since unicode is up to 4 bytes ? 
        std::string prefix = ""; // a prefix shared by this node and all descendants

        // TrieNode() = default;
        // TrieNode(const std::string &word) {

        // }

        // TrieNode *findChild() {

        // }

        // int findInsertIdx() {

        // }

        // bool hasPrefix(){
        //     return this->prefix.length() != 0;
        // }

        // void transferChiildren(TrieNode *other_node) {

        // }

    };

private:
    TrieNode root;
    int node_count = 0; // for debugging
    int word_count = 0; // count of unique words
};