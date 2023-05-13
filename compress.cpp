#include <iostream>
#include <fstream>


#include "compressors.hpp"


int main(){

    std::vector<uint> testVector = {1,2,0,3,4,5,6,2,3,52,1};
    std::ofstream testOut=std::ofstream("TestOut.dat",std::ofstream::binary);

    Compressor compressObject(testOut, testVector.size());
    std::cout<<compressObject.compressorChars.max_code<<" "<<compressObject.compressorChars.one_half<<" "<<compressObject.compressorChars.one_fourth<<std::endl;
    std::string encodedString = compressObject.encode(testVector);

    std::cout<<encodedString<<std::endl;

    Decompressor decompressObject(encodedString,compressObject.lookupTable, compressObject.totalCount);
    decompressObject.decode();
    for(uint i; i<testVector.size();i++){
        assert(decompressObject.imageValues[i]==testVector[i]);
    }
    return 0;
}