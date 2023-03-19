#include "utils.hpp"

#include <set>

using std::vector, std::set;

const vector<countValue> createSortedHistogram(const vector<unsigned int> imageValues){
    set<unsigned int> presentValues;
    for(auto val: imageValues){
        presentValues.insert(val);
    }
    vector<countValue> outCounts(0);
    for(auto val: presentValues){
        outCounts.push_back(std::tuple(val,0));
    }
    for(auto val: imageValues){
        auto it = std::find(presentValues.begin(),presentValues.end(),val);
        std::get<1>(outCounts[std::distance(presentValues.begin(),it)])+=1;
    }
}