#include <iostream>
#include <fstream>


#include "compressors.hpp"


int main(){

    std::vector<unsigned int> testVector = {0,1,2,3,4,5,6};
    std::ofstream testOut=std::ofstream("TestOut.dat",std::ofstream::binary);

    Compressor<int> compressObject(testOut, testVector.size());
    compressObject.encode(testVector);

    return 0;
}