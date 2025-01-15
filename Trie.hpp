#include <array>
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

    bool search(const std::string &prefix);
    bool getCompletions(const std::string &str, std::vector<std::string> &out_completions);
    void insert(const std::string &prefix);
    void remove(const std::string &prefix);
    void clear();

    struct TrieNode {
        bool is_leaf = false; // most nodes don't terminate words
        int child_count = 0;
        std::array<TrieNode*, 26> children = {nullptr};
    };

private:
    TrieNode root;
    int node_count = 0; // for debug purposes
};