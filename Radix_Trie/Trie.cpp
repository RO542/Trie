#include "Trie.hpp"


Trie::Trie() {
    // the root node is always a leaf as it begins but never terminates any inserted string
    root.ends_word = false;
}

Trie::Trie(const std::vector<std::string> &string_list) {
    for (const std::string &s : string_list) {
        this->insert(s);
    }
}

Trie::~Trie() {
    // std::cout << "Trie Destructor\n";
    // this->clear();
}

size_t Trie::wordCount() const {
    return this->word_count;
}

size_t Trie::nodeCount() const {
    return this->node_count;
}

bool Trie::isEmpty() const {
    return this->wordCount() == 0;
}

void Trie::insert(const std::string &word) {
    TrieNode *curr_node = &root;

    size_t word_len = word.length();
    if (word_len == 0) {
        std::cout << "You can not insert an empty string into the trie\n";
        return;
    }

    size_t i = 0;
    while (i < word_len) {
        char c = std::tolower(word[i]);
        auto kv = curr_node->children.find(c);
        if (kv == curr_node->children.end()) {
            TrieNode *node = new TrieNode();
            node_count++;
            node->prefix = word.substr(i + 1);
            node->ends_word = true;
            curr_node->children[c] = node;
            word_count++;
            return;
        }

        // skip already matched char, begin comparison on the node it indexed
        curr_node = kv->second;
        i++; 
        size_t prefix_len = curr_node->prefix.length();
        if (prefix_len == 0) {
            continue;
        }

        std::string word_slice;
        if (i + prefix_len > word_len) {
            // This is a partial match case
            word_slice = word.substr(i);
        } else {
            word_slice = word.substr(i, prefix_len);
            if (word_slice == curr_node->prefix) {
                // std::cout << "Full match prefix case \n";
                if (i + prefix_len == word_len) {
                    // std::cout << "Duplicate Word Insertion -> " << word << "\n";
                    curr_node->ends_word = true;
                    return;
                }
                // std::cout << "Remainder to Insert " << word.substr(i + prefix_len) << " from " << word << "\n";
                i += prefix_len;
                continue; // to next child 
            }
        }

        size_t div_idx = 0;
        for (size_t j = 0; j < std::min(word_len - i, prefix_len); j++) {
            if (word[i + j] == curr_node->prefix[j]) {
                div_idx++;
            } else {
                break;
            }
        }
        // std::cout << "diverge index " << idx << "\n";
        std::string new_common_prefix = curr_node->prefix.substr(0, div_idx);
        // these two are keys to sibling nodes descending from curr_node
        char split_child_key = curr_node->prefix[div_idx];

        TrieNode *split_node = new TrieNode();
        node_count++;
        split_node->prefix = curr_node->prefix.substr(div_idx + 1);
        split_node->ends_word = curr_node->ends_word;
        std::swap(curr_node->children, split_node->children);

        curr_node->prefix = new_common_prefix;
        curr_node->ends_word = false;
        curr_node->children[split_child_key] = split_node;

        char new_child_key = word[i + div_idx];
        if (new_child_key) {
            // std::cout << "new child key " << new_child_key << "\n";
            TrieNode *new_node = new TrieNode();
            node_count++;
            new_node->prefix = word.substr(i + div_idx + 1);
            new_node->ends_word = true;
            word_count++;
            curr_node->children[new_child_key] = new_node;
            // std::cout << "new child prefix " << new_node->prefix << "\n";
        } else {
            // if node is split but the key was "" it means it's characters end at curr_node->prefix 
            // therefore the prefix ends a word 
            curr_node->ends_word = true;
        }
        return;
    }
}

bool Trie::contains(const std::string &word) const  {
    const TrieNode *curr_node = &root;
    size_t word_len = word.length();
    for (size_t i = 0; i < word_len; /* manual */) {
        char c = std::tolower(word[i]);
        auto kv = curr_node->children.find(c);
        if (kv == curr_node->children.end()) {
            // std::cout << "Failed key lookup " << c << "\n";
            return false;
        }
        curr_node = kv->second;
        ++i;
        if (!curr_node->prefix.empty()) {
            size_t max_pref_overlap = curr_node->prefix.length();
            std::string slice = word.substr(i, max_pref_overlap); 
            if (slice != curr_node->prefix) {
                // std::cout << "failed at prefix matching \n";
                return false;
            }
            i += max_pref_overlap;
        }
    }
    return curr_node->ends_word;
}



void Trie::clear() {
    std::stack<TrieNode *> node_stack;
    // the root cannot be deleted because it's not dynamically allocated
    // appending all the non null children of the root sidesteps having to check for the root in the stack
    for (auto [c, node] : root.children) {
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
    word_count = 0; // should be
    assert(node_count == 0);
}

bool Trie::getCompletions(const std::string &prefix, std::vector<std::string> &out_completions) const {
    const TrieNode *curr_node = &root;
    std::cout << "Finding completions for prefix: " << prefix << "\n";

    size_t word_len = prefix.length();
    size_t i = 0;
    //FIXME:: fix traversal to end of input prefix, currently all completions from root are printed
    // while (i < word_len) {
    //     char c = std::tolower(prefix[i]);
    //     auto kv = curr_node->children.find(c);
    //     if (kv == curr_node->children.end()) {
    //         std::cout << "Failed key lookup " << c << "\n";
    //         return false;
    //     }
    //     curr_node = kv->second;
    //     ++i;
    //     if (curr_node->prefix != prefix.substr(i, curr_node->prefix.length())) {
    //         std::cout << "Failed to match prefix: " << curr_node->prefix << " with " << prefix.substr(i) << "\n";
    //         return false;
    //     }
    //     i += curr_node->prefix.length();
    // }
    std::stack<std::pair<std::string, const TrieNode *>>node_stack;
    node_stack.push({"", curr_node});
    while (!node_stack.empty()) {
        auto [suffix, node] = node_stack.top();
        node_stack.pop();
        if (node->ends_word) {
            // std::cout << "completion: " << prefix +  suffix << "\n";
            out_completions.push_back(prefix + suffix);
        }
        for (const auto [c, child] : node->children) {
            node_stack.push({suffix + c + child->prefix, child});
        }
    }
    return true;
}

void Trie::remove(const std::string &string) {

    /*
    TODO:
    if (string.empty()) {
        std::cout << "Unable to remove empty string from Trie.\n";
        return;
    }
    TrieNode *node = &root;
    std::vector<std::pair<char, TrieNode*>> path;
    for (char c : string) {
        char key = std::tolower(c);
        auto kv = node->children.find(key);
        if (kv == node->children.end()) {
            std::cout << "Prefix:  " << string << " can't be removed since it doesn't exist\n";
            return;
        }
        path.push_back({kv->first, kv->second});
        node = kv->second;
    }
    node->ends_word = false;
    word_count--;

    for (int i = path.size() - 1; i > 0; i--) {
        //c2 is the key of the path node in it's parent
        auto [child_key, child] = path[i];
        auto [_, parent] = path[i - 1];
        if (child->children.empty() && !child->ends_word) {
            delete child;
            parent->children.erase(child_key);
            node_count--;
        } else {
            // if a node has children or is not a leaf that node the input string is a substring of a greater string
            // we return to keep from disturbing the remaining nodes in the stack which are shared by both
            return;
        }
    }
    */
}