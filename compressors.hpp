#ifndef COMPRESSORS_H
#define COMPRESSORS_H

#include <limits>
#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <iostream>
#include <bitset>

#include "io.hpp"
#include "utils.hpp"

using std::numeric_limits, std::get;

struct SymbolDetails{
    uint32_t low;
    uint32_t high;
    uint32_t count;
};

class CompressorChars{
    public:
    const unsigned int precision = numeric_limits<uint32_t>::digits;
    const unsigned int code_value_bits = 16;
    const unsigned int frequency_bits = precision - this->code_value_bits;   

    const uint32_t max_code = (uint32_t(1)<<(this->code_value_bits)) -1;
    const uint32_t max_frequency = ((uint32_t)(1)<<(this->frequency_bits)) - 1;

    const uint32_t one_fourth = uint32_t(1)<<(this->code_value_bits-2);
    const uint32_t one_half = 2* this->one_fourth;
    const uint32_t three_fourths = 3 * this->one_fourth;
};

using symbolLookup = std::tuple<unsigned int, unsigned int>;

class CodingStruct{
    public:
    CodingStruct() {};
    CodingStruct(const unsigned int size) {};
    CompressorChars compressorChars;
    std::vector<unsigned int> imageValues;
    std::unordered_map<unsigned int, symbolLookup> lookupTable;
    void createLookupTable();

    int totalCount;
};

class Compressor: public CodingStruct{
    public:
    Compressor(std::ofstream& outStream, const unsigned int size): CodingStruct(size) {};

    std::string encode(std::vector<unsigned int>& inValues);

    OutCacheString outString;

};


class Decompressor: public CodingStruct{
    public:
    Decompressor(std::string inCache, std::unordered_map<unsigned int, symbolLookup> lookupTable, int totalCount): inString(InCacheString(inCache)){ this->lookupTable = lookupTable; this->totalCount = totalCount; imageValues = {};};
    void decode();
    symbolLookup getSymbol(uint scaledValue, int& outValue);

    InCacheString inString;

};


symbolLookup Decompressor::getSymbol(uint scaled_value, int& outValue){
    for(auto val: this->lookupTable){
        symbolLookup currentSymbol = get<1>(val);
        if(scaled_value<get<1>(currentSymbol)){
            outValue = get<0>(val);
            return currentSymbol;
        }
    }
}


void CodingStruct::createLookupTable(){
    std::unordered_map<unsigned int, symbolLookup> outLookup;
    std::vector<countValue> sortedCounts = createSortedHistogram(this->imageValues);
    unsigned int runningCount(0);
    for(auto symbolCount: sortedCounts){
        outLookup[get<0>(symbolCount)]=std::make_tuple(runningCount, runningCount+get<1>(symbolCount));
        runningCount+=get<1>(symbolCount);
    }
    this->totalCount = runningCount;
    this->lookupTable = outLookup;
};


std::string Compressor::encode(std::vector<unsigned int>& inValues){
    this->imageValues = inValues;
    this->createLookupTable();
    uint32_t high = this->compressorChars.max_code;
    uint32_t low = 0;
    uint32_t value = 0;
    uint64_t pending_bits(0);
    for(auto val : inValues){
        symbolLookup symbol = this->lookupTable.at(val);
        uint32_t range = high - low +1;
        high = low + (range * get<1>(symbol))/this->totalCount -1;
        low = low + (range *get<0>(symbol))/this->totalCount;
        assert(low<high);
        int failureCount(0);
        while (true) {
            if (high < this->compressorChars.one_half) {
                this->outString.append_bit_and_pending(0, pending_bits);
            } else if (low >= this->compressorChars.one_half) {
                this->outString.append_bit_and_pending(1, pending_bits);
            } else if (low >= this->compressorChars.one_fourth && high < this->compressorChars.three_fourths) {
                pending_bits++;
                low -= this->compressorChars.one_fourth;
                high -= this->compressorChars.one_fourth;
            } else {
                break;
            }
            high<<=1;
            high++;
            low<<=1;
            high &= this->compressorChars.max_code;
            low &= this->compressorChars.max_code;

        }
        
    }
    pending_bits++;
    if( low < this->compressorChars.one_fourth){
        this->outString.append_bit_and_pending(0, pending_bits);
    } else{
        this->outString.append_bit_and_pending(1, pending_bits);
    }

    this->outString.flush();
    return this->outString.out;
};

void Decompressor::decode(){
    uint32_t high = this->compressorChars.max_code;
    uint32_t low = 0;
    uint32_t value = 0;
    this->inString.initialize(value, this->compressorChars.code_value_bits);
    int outValue(0);
    for(int i=0;i<this->totalCount;++i){
        uint32_t range = high - low +1 ;
        uint32_t scaled_value = ((value - low +1)* this->totalCount-1)/range;
        symbolLookup symbol = this->getSymbol(scaled_value, outValue);
        imageValues.push_back(outValue);


        high = low + (range * get<1>(symbol))/this->totalCount -1;
        low = low + (range *get<0>(symbol))/this->totalCount;

        while (true) {
            if(high < this->compressorChars.one_half){
            } else if (low >= this->compressorChars.one_half){
                value -= this->compressorChars.one_half;
                low -= this->compressorChars.one_half;
                high -= this->compressorChars.one_half;
            } else if (low>= this->compressorChars.one_fourth && high <                     this->compressorChars.three_fourths){
                value -= this->compressorChars.one_fourth;
                low -= this->compressorChars.one_fourth;
                high -= this->compressorChars.one_fourth;
            } else break;
            low<<=1;
            high <<=1;
            high++;
            this->inString.get(value);
        }
    }
}


#endif