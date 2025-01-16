#include "Trie.hpp"


Trie::Trie() {
    // the root node is always a leaf as it begins but never terminates any inserted string
}

Trie::Trie(const std::vector<std::string> &string_list) {
    for (const std::string &s : string_list) {
        this->insert(s);
    }
}

Trie::~Trie() {
    std::cout << "Trie Destructor\n";
    this->clear();
}

bool Trie::isEmpty() {
    return root.children.empty();
}

void Trie::insert(const std::string &prefix) {
    if (prefix.empty()) {
        return;
    }
    TrieNode *curr_node = &root;
    //TODO: clean/normalize the string somehow
    for (const char c : prefix) {
        char key = std::tolower(c);
        auto pair = curr_node->children.find(key);
        if (pair == curr_node->children.end()) {
            curr_node->children[key] = new TrieNode(); 
            node_count++;
        }
        curr_node = curr_node->children[key];
    }
    curr_node->is_leaf = true; 
}

bool Trie::search(const std::string &prefix) {
    TrieNode *curr_node = &root;
    for (const char c : prefix) {
        char key = std::tolower(c);
        auto pair = curr_node->children.find(key);
        if (pair == curr_node->children.end()) {
            return false;
        }
        TrieNode *child = pair->second;
        curr_node = child;
    }
    // std::cout << "search prefix " << prefix << " -> " << (curr_node->is_leaf) << "\n"; 
    return curr_node->is_leaf;
}

void Trie::clear() {
    std::stack<TrieNode *> node_stack;
    // the root cannot be deleted because it's not dynamically allocated
    // appending all the non null children of the root sidesteps having to check for the root in the stack
    for (auto[c, node] : root.children) {
        node_stack.push(node);
    }

    while (!node_stack.empty()) {
        TrieNode *node = node_stack.top();
        node_stack.pop();
        for (auto [c, child] : node->children) {
            node_stack.push(child);
        }
        // the hashtables themselves are deleted 
        delete node;
        node_count--;
    }
    assert(node_count == 0);
}

bool Trie::getCompletions(const std::string &prefix, std::vector<std::string> &out_completions) {
    TrieNode *curr_node = &root;
    std::cout << "Finding completions: " << prefix << "\n";
    for (const char c : prefix) {
        char key = std::tolower(c);
        auto pair = curr_node->children.find(key);
        if (pair == curr_node->children.end()) {
            std::cout << "Unable to find compltions for: " << prefix << "in trie.\n";
            return false;
        }
        curr_node = pair->second;
    }

    std::stack<std::pair<std::string, TrieNode *>>node_stack;
    node_stack.push({"", curr_node});
    while (!node_stack.empty()) {
        auto [suffix, node] = node_stack.top();
        node_stack.pop();
        if (node->is_leaf) {
            std::string completion =  prefix + suffix;
            // std::cout << "completion: " << completion << "\n";
            out_completions.push_back(completion);
        }
        for (auto[c, child] : node->children) {
            node_stack.push({suffix + c,  child});
        }
    }
    return true;
}

void Trie::remove(const std::string &string) {
    if (string.empty()) {
        std::cout << "Unable to remove empty string from Trie.\n";
        return;
    }
    TrieNode *node = &root;
    std::vector<std::pair<char, TrieNode*>> path;
    for (const char c : string) {
        char key = std::tolower(c);
        auto kv = node->children.find(key);
        if (kv == node->children.end()) {
            std::cout << "Prefix:  " << string << " can't be removed since it doesn't exist\n";
            return;
        }
        path.push_back({kv->first, kv->second});
        node = kv->second;
    }
    node->is_leaf = false;

    for (int i = path.size() - 1; i > 0; i--) {
        //c2 is the key of the path node in it's parent
        auto [child_key, child] = path[i];
        auto [_, parent] = path[i - 1];
        if (child->children.empty() && !child->is_leaf) {
            delete child;
            parent->children.erase(child_key);
            node_count--;
        } else {
            // if a node has children or is not a leaf that node the input string is a substring of a greater string
            // we return to keep from disturbing the remaining nodes in the stack which are shared by both
            return;
        }
    }
}
