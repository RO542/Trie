#include "Trie.hpp"

Trie::Trie() {
    root.is_leaf = true;
}

Trie::Trie(const std::vector<std::string> &string_list) {
    for (const std::string &s : string_list) {
        this->insert(s);
    }
}

Trie::~Trie() {
    // std::cout << "Trie Destructor\n";
    this->clear();
}

void Trie::insert(const std::string &prefix) {
    if (prefix.empty()) return;
    TrieNode *curr_node = &root;
    for (const char c : prefix) {
        // ignore non ascii chars for now 
        if (!std::isalpha(c)) {
            continue;
        }
        const char c_lower = std::tolower(c);
        int idx = c_lower - 'a';
        if (!curr_node->children[idx]) {
            curr_node->children[idx] = new TrieNode();
            node_count++;
        }
        curr_node->child_count++;
        curr_node = curr_node->children[idx];
    }
    curr_node->is_leaf = true; // false by default
}

bool Trie::search(const std::string &prefix) {
    TrieNode *curr_node = &root;
    for (const char c : prefix) {
        int idx = std::tolower(c) - 'a';
        TrieNode *child = curr_node->children[idx]; 
        if (!child) {
            std::cout << "ended match during search\n";
            return false; // prefix mismatch
        }
        curr_node = child;
    }
    std::cout << "search prefix " << prefix << " -> " << (curr_node->is_leaf) << "\n"; 
    return curr_node->is_leaf;
}

void Trie::clear() {
    std::stack<TrieNode *> node_stack;
    // the root cannot be deleted because it's not dynamically allocated
    // append all the non null children of the root sidesteps having to check for the root in the while loop
    for (TrieNode *child : root.children) {
        if (child) {
            node_stack.push(child);
        }
    }
    while (!node_stack.empty()) {
        TrieNode *node = node_stack.top();
        node_stack.pop();
        // collect child node references before deleting the current node reference
        for (TrieNode *child : node->children) {
            if (child)  {
                node_stack.push(child);
            }
        }
        delete node;
        node_count--;
    }
    assert(node_count == 0);
}
bool Trie::getCompletions(const std::string &prefix, std::vector<std::string> &out_completions) {
    if (prefix.empty()) {
        std::cout << "Cannot search for empty string\n";
        return false;
    }

    TrieNode *curr_node = &root;
    std::cout << "Finding completions: " << prefix << "\n";
    for (const char c : prefix) {
        const char lower_c = std::tolower(c);
        int idx = lower_c - 'a';
        TrieNode *child = curr_node->children[idx]; 
        if (!child) {
            return false;
        }
        curr_node = child;
    }

    std::stack<std::pair<TrieNode *, std::string>> node_stack;
    node_stack.push({curr_node, ""});
    while (!node_stack.empty()) {
        auto [node, suffix] = node_stack.top();
        node_stack.pop();
        if (node->is_leaf) {
            std::cout << "Completion: " << prefix + suffix << "\n"; 
            out_completions.push_back(prefix + suffix);
        }
        for (int i = 0; i < 26; i++) {
            TrieNode *child = node->children[i];
            if (child) {
                node_stack.push(
                    {child, suffix + static_cast<char>(i + 'a')}
                );
            }
        }
    }
    return true;
}

void Trie::remove(const std::string &prefix) {
    if (prefix.empty()) {
        std::cout << "Unable to remove empty string from Trie.\n";
        return;
    }
    TrieNode *node = &root;
    std::vector<TrieNode *> path;
    for (const char c : prefix) {
        int idx = std::tolower(c) - 'a';
        TrieNode *child = node->children[idx];
        if (!child) {
            std::cout << "Input: " << prefix << " does not exist as a prefix.\n";
            return;
        }
        path.push_back(child);
        node = child;
    }
    node->is_leaf = false;

    for (int i = path.size() - 1; i > 0; i--) {
        TrieNode *child = path[i];
        TrieNode *parent = path[i - 1];
        if (child->child_count == 0 && !child->is_leaf) {
            delete child;
            int idx = std::tolower(prefix[i]) - 'a';
            parent->children[idx] = nullptr;
            parent->child_count--;
            node_count--;
        } else {
            // when a node does have children this means our prefix is a substring of a larger string
            // this makes retaining this node and all it's ancestors going up to the root necessary
            return;
        }
    }
}
