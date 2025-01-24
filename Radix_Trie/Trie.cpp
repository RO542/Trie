#include "Trie.hpp"


Trie::Trie() {
    root.ends_word = false; // root never terminates a word or has a prefix of it's own
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
    // debug function
    return this->node_count;
}

bool Trie::isEmpty() const {
    return this->wordCount() == 0;
}

void Trie::insert(const std::string &word) {
    size_t word_len = word.length();
    if (word_len == 0) {
        std::cout << "You can not insert an empty string into the trie\n";
        return;
    }
    TrieNode *curr_node = &root;
    char c;
    size_t char_idx = 0;
    while (char_idx < word_len) {
        c = std::tolower(word[char_idx]);
        auto kv = curr_node->children.find(c);
        if (kv == curr_node->children.end()) {
            // if a key lookup fails a node is it's own parent at this tree level
            TrieNode *node = new TrieNode();
            node_count++;
            node->prefix = word.substr(char_idx + 1);
            node->ends_word = true;
            curr_node->children[c] = node;
            word_count++;
            return;
        }

        curr_node = kv->second;
        char_idx++; 
        size_t prefix_len = curr_node->prefix.length();
        if (prefix_len == 0) {
            continue;
        }

        size_t max_prefix_overlap = std::min(prefix_len, word_len);
        std::string word_slice = word.substr(char_idx, max_prefix_overlap);
        size_t div_idx = 0; // index where the current prefix and relevant portion of word diverge
        for (div_idx = 0; div_idx < word_slice.length(); div_idx++) {
            if (curr_node->prefix[div_idx] != word_slice[div_idx]) {
                break;
            }
        }

        // full prefix matches either if EOW mark it, otherwise continue inserting
        if (div_idx == prefix_len) { 
            // prefix terminates word 
            if (char_idx + prefix_len == word_len) {
                curr_node->ends_word = true;
                return;
            }
            char_idx += prefix_len;
            continue;
        }

        // below are node prefix splitting cases
        char split_key = curr_node->prefix[div_idx];
        std::string split_prefix = curr_node->prefix.substr(div_idx + 1);
        TrieNode *split_node = new TrieNode();
        node_count++;
        split_node->prefix = split_prefix;
        split_node->ends_word = curr_node->ends_word;

        // transfer curr_node properties to split child
        std::swap(split_node->children, curr_node->children);
        curr_node->prefix = curr_node->prefix.substr(0, div_idx);
        curr_node->ends_word = false;
        curr_node->children[split_key] = split_node;
        
        if (char_idx + div_idx == word_len) {
            // prefix terminates word after splitting
            std::cout << "EOW at curr during split\n";
            curr_node->ends_word = true;
            return;
        }

        // now that a common prefix is guaranteed, the remaining portion of word becomes a compressed node
        char new_child_key = word[char_idx + div_idx];
        TrieNode *new_node = new TrieNode();
        node_count++;
        new_node->prefix = word.substr(char_idx + div_idx + 1);
        new_node->ends_word = true;
        word_count++;
        curr_node->children[new_child_key] = new_node;
        return;
        // can the above insertion be replace with char_idx += div_idx ? 
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
    char c;
    while (i < word_len) {
        c = std::tolower(prefix[i]);
        auto kv = curr_node->children.find(c);
        if (kv == curr_node->children.end()) {
            std::cout << "Failed key lookup " << c << "\n";
            return false;
        }
        curr_node = kv->second;
        ++i;

        size_t prefix_len = curr_node->prefix.length();
        size_t max_overlap = std::min(prefix_len, word_len);
        size_t div_idx = 0;
        std::string slice = prefix.substr(i, max_overlap);
        for (size_t div_idx = 0; div_idx < max_overlap; div_idx++) {
            if (slice[div_idx] != curr_node->prefix[div_idx]) {
                break;
            }
        }
        // if div_idx is 0, we already matched c so curr_node is the root of completions 
        // if partial prefix match break
        // if full prefix match goto next node
        if (div_idx == i + prefix_len)
            i += curr_node->prefix.length();
        else if (div_idx >= 0) 
            break;
    }

    std::stack<std::pair<std::string, const TrieNode *>>node_stack;
    node_stack.push({curr_node->prefix, curr_node});
    while (!node_stack.empty()) {
        auto [suffix, node] = node_stack.top();
        node_stack.pop();
        if (node->ends_word) {
            std::cout << "completion: " << prefix +  suffix << "\n";
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