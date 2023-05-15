<p>

  <h3 align="center">Medical Imaging Compression with Stationary Wavelets and Arithmetic Encoding (Miswae)</h3>
</p>
![Example](logo.png)

## Table of contents

- [Quick start](#quick-start)
- [Status](#status)
- [What's included](#whats-included)
- [Bugs and feature requests](#bugs-and-feature-requests)
- [Contributing](#contributing)
- [Creators](#creators)
- [Thanks](#thanks)
- [Copyright and license](#copyright-and-license)


## Quick start

This is a python package for compressing integer coded imaging data. A modern C++ and Python installation is required; in addition, numpy, pybind, blitz, and blitzwave are needed. All are found readily on either brew (on macOS) or apt (for Linux). Windows users, as always, are left to their own devices. It is recommended to install WSL and simply interface with apt throught that distribution.

The Makefile is set up to link the libraries as they appear on a Mac installation of Python and pybind; it should be relatively straightforward what to change where. Consult this [document](https://pybind11.readthedocs.io/en/stable/compiling.html) should all else fail for building pybind. 

Instructions are simply 

- Run `make`

Two dynamically linked python libraries are generated: `blitzTry` and `compress` along with their typehints. 


## What's included

All of the important files are in `src` directory.

```text
src/
└── Makefile
    blitzInterface.cpp
    compress.cpp
    ...
```

A copy of `lena` is included for testing purposes.

## Description

This is a work in progress implementation of a medical image compression algorithm as described in this [paper](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4171424/). They claim compression ratios of 17.5 for their method.

At the moment, the implemented steps are a baseline arithmetic encoder, and the stationary wavelet transform. I have not yet implemented the EBCOT encoder proposed in the paper, but that should come soon. As it stands, the compression is on par with EBCOT alone. Compressing a little section of Lena gets a CR of about 8, so that's pretty nice. More work to come.

## Bugs and feature requests

This work is ongoing; there are bugs. For sure. 😂



## Creators

**Creator 1**

- <github.com/whitman103>

## Thanks

Readme template stolen blatantly from [lsmaestro](https://github.com/Ismaestro/markdown-template). Many thanks and regards

## Copyright and license

All copyright comes from above. You can use whatever I've done here as you'd like. 