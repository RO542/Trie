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
    size_t wordCount() const;// for testing/debugging
    size_t nodeCount() const;// for testing/debugging
    bool contains(const std::string &prefix) const;
    bool getCompletions(const std::string &str, std::vector<std::string> &out_completions) const;
    void insert(const std::string &prefix);
    void remove(const std::string &prefix);
    void clear();
    bool isEmpty() const;

    struct TrieNode {
        bool ends_word = false; // most nodes don't terminate words
        std::unordered_map<char, TrieNode *> children;
    };

private:
    TrieNode root;
    int node_count = 0; // for debugging
    size_t word_count = 0;
};

