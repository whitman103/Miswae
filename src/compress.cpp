#include <iostream>
#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "compressors.hpp"

namespace pb = pybind11;

using NpIntArray = pb::array_t<unsigned int, pb::array::c_style>;

struct CompressionInterface{
    const int totalCount;
    pb::bytes compressVector(std::vector<unsigned int> inVector);
    std::vector<uint> decompressVector(pb::bytes inBinary);
    std::unordered_map<unsigned int, symbolLookup> symbolTable;
    void testBackend();
    CompressionInterface(int count): totalCount(count){};
};

pb::bytes CompressionInterface::compressVector(std::vector<unsigned int> inVector){
    Compressor compressObject(inVector.size());
    std::string encodedString = compressObject.encode(inVector);
    this->symbolTable = compressObject.lookupTable;
    return pb::bytes(encodedString);
};

std::vector<unsigned int> CompressionInterface::decompressVector(pb::bytes inBinary){
    Decompressor decompressObject(std::string(inBinary), this->symbolTable , this->totalCount);
    decompressObject.decode();
    return decompressObject.imageValues;
}

void CompressionInterface::testBackend(){
    std::vector<uint> testVector = {1,2,0,3,4,5,6,2,3,52,1};
    Compressor compressObject(testVector.size());
    std::string encodedString = compressObject.encode(testVector);

    pb::print(encodedString);
    Decompressor decompressObject(encodedString, compressObject.lookupTable, compressObject.totalCount);
    decompressObject.decode();
}

PYBIND11_MODULE(compress,m){
    pb::class_<CompressionInterface>(m,"CompressionInterface")
    .def(pb::init<int>())
    .def_readonly("totalCount",&CompressionInterface::totalCount)
    .def_readwrite("symbolTable",&CompressionInterface::symbolTable)
    .def("testBackend",&CompressionInterface::testBackend)
    .def("compressVector",&CompressionInterface::compressVector,pb::kw_only(), pb::arg("vector"))
    .def("decompressVector",&CompressionInterface::decompressVector, pb::kw_only(), pb::arg("compressed_string"));
};

