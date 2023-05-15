#include <iostream>
#include <blitzwave/Wavelet.h>
#include <blitzwave/WaveletDecomp.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace pb = pybind11;

using ShortNpArray = pb::array_t<int, pb::array::c_style>;

void simpleDecompose(short *data, int rows, int cols);

void simpleDecompose(short *data, int rows, int cols){
    using namespace blitz;
    using namespace bwave;

    GeneralArrayStorage<2> storage;
    storage.ordering() = secondDim, firstDim;
    Array<short, 2> array(data, shape(rows,cols), neverDeleteData, storage);

    WaveletDecomp<2> decomp(WL_HAAR, NONSTD_DECOMP);
    decomp.apply(array);
}

void npDecompose(ShortNpArray& inData);

void npDecompose(ShortNpArray& inData){
    using namespace blitz;
    using namespace bwave;

    GeneralArrayStorage<2> storage;
    storage.ordering() = secondDim, firstDim;
    auto dataBuffer = inData.request();
    Array<int, 2> array((int*)dataBuffer.ptr, shape(dataBuffer.shape[0],dataBuffer.shape[1]), neverDeleteData, storage);

    WaveletDecomp<2> decomp(WL_HAAR, NONSTD_DECOMP);
    decomp.apply(array);
};

void npRecompose(ShortNpArray& inData){
    using namespace blitz;
    using namespace bwave;

    GeneralArrayStorage<2> storage;
    storage.ordering() = secondDim, firstDim;
    auto dataBuffer = inData.request();
    Array<int,2> array((int*)dataBuffer.ptr, shape(dataBuffer.shape[0],dataBuffer.shape[1]),neverDeleteData,storage);

    WaveletDecomp<2> decomp(WL_HAAR, NONSTD_DECOMP);
    decomp.applyInv(array);
}


PYBIND11_MODULE(blitzTry, m){
    m.def("decompose",&npDecompose)
    .def("recompose",&npRecompose);
}