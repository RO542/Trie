#include <iostream>
#include <vector>

int binary_search_insert_idx(int target, const std::vector<int> &vec) {
    // find the index nearest to target
    int l = 0, r = vec.size() - 1; 
    int m;
    while (l <= r) {
        m = (l + r) / 2;
        if (vec[m] < target) {
            l = m + 1; 
        } else {
            r = m - 1;
        }
    }
    return m;
}


int binary_search_find(int target, const std::vector<int> &vec) {
    // find the index nearest to target
    int l = 0, r = vec.size() - 1; 
    int m;
    while (l <= r) {
        m = (l + r) / 2;
        if  (vec[m] == target) {
            return m;
        } else if (vec[m] < target) {
            l = m + 1; 
        } else {
            r = m - 1;
        }
    }
    return -1;
}
