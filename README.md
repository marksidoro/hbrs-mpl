# `hbrs-mpl`
[![Build Status](https://travis-ci.com/JM1/hbrs-mpl.svg?branch=master)](https://travis-ci.com/JM1/hbrs-mpl)

`hbrs-mpl` is a experimental generic [C++17](https://en.wikipedia.org/wiki/C++17) library for distributed scientific computing at HPC clusters.
Our research goal is to codify complex algorithms like [singular value decomposition (SVD)](https://en.wikipedia.org/wiki/Singular_value_decomposition) using abstract, mathematical notations without sacrificing space and time efficiency.
We use meta programming to [evaluate expressions at compile-time](https://github.com/JM1/hbrs-mpl/blob/expression_evaluation_framework_2/src/hbrs/mpl/core/evaluate/impl.hpp) and thus avoid negative impact on run-time.
We apply generic techniques with the help of C++ templates to write reusable and robust components.

Our primary show case is [`hbrs-theta_utils`](https://github.com/JM1/hbrs-theta_utils/), a postprocessing tool to CFD solver [TAU (and THETA)](http://tau.dlr.de/) for data-driven modal decompositions.

:warning: **WARNING:** As usual for most research projects, be warned that this code is not ready for production, is heavily under development, might change at any time and is missing proper documentation :warning:

## History

Its development started in 2015 as a research project at Bonn-Rhein-Sieg University of Applied Sciences, from 2016-2019 it was funded by BMBF project [AErOmAt](https://www.h-brs.de/de/aeromat) and till today its actively developed by its initial author, Jakob Meng.

## Under the hood

- `hbrs-mpl` provides algorithms for 
   - linear algebra ([singular value decomposition (SVD)](https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/svd))
   - statistics ([Principal Component Analysis (PCA) / Proper Orthogonal Decomposition (POD)](https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/pca))
   - fluid mechanics ([Dynamic Mode Decomposition (DMD)](https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/dmd))
- `hbrs-mpl` provides local and distributed [datastructures](https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/dt) for matrices, vectors and sequences
- `hbrs-mpl`'s [mathematical functions](https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn) are mostly geared towards compatibility with [MATLAB's API](https://de.mathworks.com/help/), because the latter has a strong focus on mathematical notations, is properly documented and useful for rapid prototyping.
- `hbrs-mpl` heavily relies on [Elemental](https://github.com/elemental/Elemental) for linear algebra algorithms, that we have not yet implemented as generic and functional code. 3rd party code from Elemental and MATLAB is wrapped and hidden from the user.
- With [MATLAB Coder](https://de.mathworks.com/products/matlab-coder.html), we generate C code from our MATLAB code and use it in our unit tests to compare results of our algorithms to MATLAB or rather its LAPACK libraries. But our code compiles fine without MATLAB!
- We extensively use [Boost.Hana](https://github.com/boostorg/hana) for our [meta programming tasks](https://github.com/JM1/hbrs-mpl/blob/expression_evaluation_framework_2/src/hbrs/mpl/core/evaluate/impl.hpp).
- [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface) is our favored API for 
   [distributed](https://github.com/JM1/hbrs-theta_utils/blob/master/src/hbrs/theta_utils/detail/vtk/impl.cpp)
   [computations](https://github.com/JM1/hbrs-theta_utils/blob/master/src/hbrs/theta_utils/detail/scatter/impl.cpp).
- All unit tests (e.g. for 
   [svd](https://github.com/JM1/hbrs-mpl/blob/master/src/hbrs/mpl/fn/svd/test/hbrs_mpl.cpp) and
   [pca](https://github.com/JM1/hbrs-mpl/blob/master/src/hbrs/mpl/fn/pca/test/hbrs_mpl.cpp))
   are written with [Boost.Test](https://www.boost.org/doc/libs/release/libs/test/).
- [CMake 3](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) is used to [build, export and install our library](https://github.com/JM1/hbrs-mpl/blob/master/CMakeLists.txt)
- We use [Travis CI](https://travis-ci.com/JM1/hbrs-mpl) to build and test our code against different compilers.

# How to build this code using Docker

```sh
# install Docker CE for Debian or derivatives
# please follow guide at https://docs.docker.com/install/linux/docker-ce/debian/

# docker version 18.06.0-ce or later is recommended
docker --version

# fetch docker image
docker pull jm1337/debian-dev-hbrs:buster

# log into docker container
docker run -ti jm1337/debian-dev-hbrs:buster



# the following commands are executed from within the docker container

# fetch, compile and install prerequisites
git clone --depth 1 https://github.com/JM1/hbrs-cmake.git
cd hbrs-cmake
mkdir build && cd build/
# install to non-system directory because sudo is not allowed in this docker container
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
make -j$(nproc)
make install
cd ../../

# fetch, compile and install hbrs-mpl
git clone --depth 1 https://github.com/JM1/hbrs-mpl.git
cd hbrs-mpl
mkdir build && cd build/
cmake \
 -DCMAKE_INSTALL_PREFIX=$HOME/.local \
 -DHBRS_MPL_ENABLE_TESTS=ON \
 -DHBRS_MPL_ENABLE_BENCHMARKS=ON \
 ..
make -j$(nproc)
ctest --output-on-failure
make install
```

For more examples on how to build and test this code see [`.travis.yml`](https://github.com/JM1/hbrs-mpl/blob/master/.travis.yml).
