#include <iostream>
#include <vector>

int sorted_arr_insert(int target, std::vector<int> &vec) {
    // find the index nearest to target
    int l = 0, r = vec.size() - 1; 
    while (l <= r) {
        int m = (l + r) / 2;
        if (vec[m] < target) {
            l = m + 1; 
        } else {
            r = m - 1;
        }
    }
    // Insert target at the position l
    vec.emplace(vec.begin() + l, target);
    return l; // Return the index where the target was inserted
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

int main() {
    std::vector<int> vec = {0, 1, 3, 4};
    std::cout << sorted_arr_insert(2, vec) << "\n";

}