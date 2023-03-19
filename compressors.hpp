#ifndef COMPRESSORS_H
#define COMPRESSORS_H

#include <limits>
#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>

#include "io.hpp"

using std::numeric_limits;

template<typename code_type>
struct SymbolDetails{
    code_type low;
    code_type high;
    code_type count;
};

template<typename code_type>
class CompressorChars{
    public:
    const unsigned int precision = numeric_limits<code_type>::digits;
    const unsigned int code_value_bits = (this->precision+3)/2;
    const unsigned int frequency_bits = precision - this->code_value_bits;   

    const code_type max_code = code_type(1)<<(this->code_value_bits) -1;
    const unsigned int max_frequency = (unsigned int)(1)<<(this->frequency_bits) - 1;

    const code_type one_fourth = code_type(1)<<(this->code_value_bits-2);
    const code_type one_half = 2* this->one_fourth;
    const code_type three_fourths = 3 * this->one_fourth;

};

using symbolLookup = std::tuple<unsigned int, unsigned int>;

template<typename code_type>
class Compressor{
    public:
    Compressor() {};
    Compressor(std::ofstream& outStream, const unsigned int size) : output_object(outStream), totalCount(size){};
    CompressorChars<code_type> compressorChars;
    std::vector<unsigned int> imageValues;
    const uint totalCount;
    const SymbolDetails<code_type> getRangeValues(const unsigned int inValue);
    std::unordered_map<unsigned int, symbolLookup> lookupTable;
    void encode(std::vector<unsigned int>& inValues);
    void createLookupTable();
    inline void put_bit_plus_pending(const bool outBit, int& pending_bits){
        this->output_object.put_bit(outBit);
        for(uint i{0};i<pending_bits;++i){
            this->output_object.put_bit(!outBit);
        }
        pending_bits=0;
    };

    private:
    output_bits output_object;
};



template<typename code_type>
const SymbolDetails<code_type> Compressor<code_type>::getRangeValues(const unsigned int inValue){
    return {0,1,this->totalCount};
};

template<typename code_type>
void Compressor<code_type>::createLookupTable(){
    using std::make_tuple;
    std::unordered_map<unsigned int, symbolLookup> valueToCount;
    for(auto& val: this->imageValues){
        if(valueToCount.find(val)==valueToCount.end()) valueToCount[val]= make_tuple(0,0);
        get<0>(valueToCount.at(val))+=1;
    }
    this->lookupTable = valueToCount;
};

template<typename code_type>
void Compressor<code_type>::encode(std::vector<unsigned int>& inValues){
    code_type high = this->compressorChars.max_code;
    code_type low = 0;
    code_type value = 0;
    int pending_bits(0);
    for(auto& val : inValues){
        SymbolDetails<code_type> symbol = this->getRangeValues(val);
        code_type range = high - low + 1;
        high = low +(range*symbol.high/symbol.count)-1;
        low = low+(range*symbol.low/symbol.count);

        while(true){
            if(high < this->compressorChars.one_half){
                put_bit_plus_pending(0, pending_bits);
            } else if (low >= this->compressorChars.one_half){
                put_bit_plus_pending(1, pending_bits);
            } else if (low >= this->compressorChars.one_fourth && high < this->compressorChars.three_fourths){
                pending_bits++;
                low -= this->compressorChars.one_fourth;
                high -= this->compressorChars.one_fourth;
            } else break;
            high <<= 1;
            high++;
            low <<= 1;
            high &= this->compressorChars.max_code;
            low &= this->compressorChars.max_code;

        }
        pending_bits++;
        if( low < this->compressorChars.one_fourth){
            put_bit_plus_pending(0, pending_bits);
        } else{
            put_bit_plus_pending(1, pending_bits);
        }
    }
};


using uIntChars = CompressorChars<unsigned int>;
using IntChars = CompressorChars<int>;
using LongIntChars = CompressorChars<long int>;
using LongUIntChars = CompressorChars<long unsigned int>;

#endif