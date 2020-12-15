# Generic C++ library `hbrs-mpl` for scientific computing at HPC clusters

`hbrs-mpl` is a experimental generic [C++17][wiki-cpp17] library ([GitHub.com][hbrs-mpl],
[H-BRS GitLab][hbrs-gitlab-hbrs-mpl]) for distributed scientific computing at HPC clusters.
Our research goal is to be able to codify complex algorithms like [singular value decomposition (SVD)][wiki-svd]
using abstract, mathematical notation without sacrificing space and time efficiency.
We use meta programming to [evaluate expressions at compile-time][hbrs-mpl-core-evaluate] and 
thus avoid any negative performance impact on run-time.
We apply generic techniques with the help of C++ templates to write reusable and robust components.

Its development started in 2015 as a research project at [Bonn-Rhein-Sieg University of Applied Sciences][hbrs],
from 2016-2019 it was funded partly by BMBF project [AErOmAt][aeromat].

Primary use cases for `hbrs-mpl` are:
* [`hbrs-theta_utils`][hbrs-theta-utils] ([GitHub.com][hbrs-theta-utils], [H-BRS GitLab][hbrs-gitlab-hbrs-theta-utils]),
  a postprocessing tool to CFD solvers [TAU and THETA][tau] for data-driven modal decompositions
* [`python-edamer`][py-edamer] ([GitHub.com][py-edamer], [H-BRS GitLab][hbrs-gitlab-py-edamer]),
  a Python 3 library for exascale data analysis and machine learning applications

:warning: **WARNING:**
This code is still under development and is not ready for production yet.
Code might change or break at any time and is missing proper documentation.
:warning:

## Under the hood

`hbrs-mpl` provides algorithms for
* linear algebra, e.g. matrix operations like 
  [`hbrs::mpl::fn::multiply`][hbrs-mpl-fn-multiply] and [`hbrs::mpl::fn::transpose`][hbrs-mpl-fn-transpose]
* numerical linear algebra, e.g.
  [eigenvalue decomposition][hbrs-mpl-fn-eig], [singular value decomposition (SVD)][hbrs-mpl-fn-svd]
* statistics, e.g. [Principal Component Analysis (PCA) / Proper Orthogonal Decomposition (POD)][hbrs-mpl-fn-pca]
* time-dependent statistics, e.g. [Dynamic Mode Decomposition (DMD)][hbrs-mpl-fn-dmd]

`hbrs-mpl`'s [functions][hbrs-mpl-fn] are mostly geared towards compatibility with [MATLAB's API][matlab-help], because
the latter has a strong focus on mathematical notations, is properly documented and useful for rapid prototyping.

`hbrs-mpl` provides local and distributed [data structures][hbrs-mpl-dt] for matrices, vectors and sequences.

`hbrs-mpl` builds heavily on [`Elemental`][elemental] for distributed data structures and linear algebra algorithms and
[`Boost.Hana`][boost-hana-ref] for [meta programming][hbrs-mpl-core-evaluate] and generic coding.

The full tech stack consists of:
* [`C++17`][cpp-ref] for generic, functional and efficient library code
* C++ library [`Elemental`][elemental]
* C++ metaprogramming library [`Boost.Hana`][boost-hana-ref]
* [MATLAB Coder][matlab-coder] to generate C code from [MATLAB code][hbrs-mpl-detail-matlab-cxn] which we then use
  exclusively in unit tests to compare results of `hbrs-mpl`'s algorithms to MATLAB or rather its LAPACK libraries.
  Note, `hbrs-mpl` compiles just fine without MATLAB!
* [MPI][wiki-mpi] for [distributed][hbrs-theta-utils-detail-vtk] [computations][hbrs-theta-utils-detail-scatter]
* [`Boost.Test`][boost-test] for unit tests, e.g. 
  [`hbrs::mpl::fn::svd`][hbrs-mpl-fn-svd-test-hbrs-mpl] and [`hbrs::mpl::fn::pca`][hbrs-mpl-fn-pca-test-hbrs-mpl]
* [CMake 3][cmake3-tut] and [`hbrs-cmake`][hbrs-cmake] to [build, export and install our library](CMakeLists.txt)
* [GitLab CI][hbrs-gitlab-hbrs-mpl-ci] to continuously build and test our code against different compilers
* [`Python 3`][python3-ref] to [generate missing headers][hbrs-mpl-misc-generate]

## How to build, install and run code using `Docker` or `Podman`

For a quick and easy start into developing with C++, a set of ready-to-use `Docker`/`Podman` images
`jm1337/debian-dev-hbrs` and `jm1337/debian-dev-full` (supports more languages) has been created. They contain a full
development system including all tools and libraries necessary to hack on distributed decomposition algorithms and more
([Docker Hub][docker-hub-jm1337], [source files for Docker images][docker-artifacts]).

### Install `Docker` or `Podman`

* On `Debian 10 (Buster)` or `Debian 11 (Bullseye)` just run `sudo apt install docker.io`
  or follow the [official install guide][docker-install-debian] for Docker Engine on Debian
* On `Ubuntu 18.04 LTS (Bionic Beaver)` and `Ubuntu 20.04 LTS (Focal Fossa)` just run `sudo apt install docker.io`
  (from `bionic/universe` and `focal/universe` repositories)
  or follow the [official install guide][docker-install-ubuntu] for Docker Engine on Ubuntu
* On `Windows 10` follow the [official install guide][docker-install-windows] for Docker Desktop on Windows
* On `Mac` follow the [official install guide][docker-install-mac] for Docker Desktop on Mac
* On `Fedora`, `Red Hat Enterprise Linux (RHEL)` and `CentOS` follow the [official install guide][podman-install] for
  Podman

### Setup and run container

```sh
# docker version 18.06.0-ce or later is recommended
docker --version

# fetch docker image
docker pull jm1337/debian-dev-hbrs:bullseye

# log into docker container
docker run -ti jm1337/debian-dev-hbrs:bullseye
# or using a persistent home directory, e.g.
docker run -ti -v /HOST_DIR:/home/devil/ jm1337/debian-dev-hbrs:bullseye
# or using a persistent home directory on Windows hosts, e.g.
docker run -ti -v C:\YOUR_DIR:/home/devil/ jm1337/debian-dev-hbrs:bullseye
```

Podman strives for complete CLI compatibility with Docker, hence
[you may use the `alias` command to create a `docker` alias for Podman][docker-to-podman-transition]:
```sh
alias docker=podman
```

### Build and run code inside container

Execute the following commands within the `Docker`/`Podman` container:

```sh
# choose a compiler
export CC=clang-10
export CXX=clang++-10
# or
export CC=gcc-10
export CXX=g++-10

# fetch, compile and install prerequisites
git clone --depth 1 https://github.com/JM1/hbrs-cmake.git
cd hbrs-cmake
mkdir build && cd build/
# install to non-system directory because sudo is not allowed in this docker container
cmake \
    -DCMAKE_INSTALL_PREFIX=$HOME/.local \
    ..
make -j$(nproc)
make install
cd ../../

# fetch, compile and install hbrs-mpl
git clone --depth 1 https://github.com/JM1/hbrs-mpl.git
cd hbrs-mpl
mkdir build && cd build/
cmake \
 -DCMAKE_INSTALL_PREFIX=$HOME/.local \
 -DHBRS_MPL_ENABLE_ELEMENTAL=ON \
 -DHBRS_MPL_ENABLE_MATLAB=OFF \
 -DHBRS_MPL_ENABLE_TESTS=ON \
 -DHBRS_MPL_ENABLE_BENCHMARKS=ON \
 ..
make -j$(nproc)
ctest --verbose --output-on-failure
make install
```

For more examples on how to build and test this code see [`.gitlab-ci.yml`](.gitlab-ci.yml).

## License

GNU General Public License v3.0 or later

See [LICENCE.md](LICENSE.md) to see the full text.

## Author

Jakob Meng
@jm1 ([GitHub.com][github-jm1], [Web][jm])

[//]: # (References)

[aeromat]: https://www.h-brs.de/de/aeromat
[boost-hana]: https://github.com/boostorg/hana
[boost-hana-ref]: https://boostorg.github.io/hana/
[boost-test]: https://www.boost.org/doc/libs/release/libs/test/
[cmake3-tut]: https://cmake.org/cmake/help/latest/guide/tutorial/index.html
[cpp-ref]: https://en.cppreference.com/w/cpp
[docker-artifacts]: https://github.com/JM1/docker-artifacts
[docker-hub-jm1337]: https://hub.docker.com/r/jm1337/
[docker-install-debian]: https://docs.docker.com/engine/install/debian/
[docker-install-mac]: https://docs.docker.com/docker-for-mac/install/
[docker-install-ubuntu]: https://docs.docker.com/engine/install/ubuntu/
[docker-install-windows]: https://docs.docker.com/docker-for-windows/install/
[docker-to-podman-transition]: https://developers.redhat.com/blog/2020/11/19/transitioning-from-docker-to-podman/
[elemental]: https://github.com/elemental/Elemental
[github-jm1]: https://github.com/jm1
[hbrs]: https://www.h-brs.de
[hbrs-gitlab-hbrs-mpl]: https://git.inf.h-brs.de/jmeng2m/hbrs-mpl/
[hbrs-gitlab-hbrs-mpl-ci]: https://git.inf.h-brs.de/jmeng2m/hbrs-mpl/-/pipelines
[hbrs-gitlab-py-edamer]: https://git.inf.h-brs.de/jmeng2m/python-edamer
[hbrs-gitlab-hbrs-theta-utils]: https://git.inf.h-brs.de/jmeng2m/hbrs-theta_utils/
[hbrs-cmake]: https://github.com/JM1/hbrs-cmake/
[hbrs-mpl]: https://github.com/JM1/hbrs-mpl/
[hbrs-mpl-core-evaluate]: https://github.com/JM1/hbrs-mpl/blob/expression_evaluation_framework_2/src/hbrs/mpl/core/evaluate/impl.hpp
[hbrs-mpl-detail-matlab-cxn]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/detail/matlab_cxn
[hbrs-mpl-dt]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/dt
[hbrs-mpl-fn]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn
[hbrs-mpl-fn-eig]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/eig
[hbrs-mpl-fn-dmd]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/dmd
[hbrs-mpl-fn-multiply]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/multiply
[hbrs-mpl-fn-pca]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/pca
[hbrs-mpl-fn-pca-test-hbrs-mpl]: https://github.com/JM1/hbrs-mpl/blob/master/src/hbrs/mpl/fn/pca/test/hbrs_mpl.cpp
[hbrs-mpl-fn-svd]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/svd
[hbrs-mpl-fn-svd-test-hbrs-mpl]: https://github.com/JM1/hbrs-mpl/blob/master/src/hbrs/mpl/fn/svd/test/hbrs_mpl.cpp
[hbrs-mpl-fn-transpose]: https://github.com/JM1/hbrs-mpl/tree/master/src/hbrs/mpl/fn/transpose
[hbrs-mpl-misc-generate]: https://github.com/JM1/hbrs-mpl/blob/master/misc/generate.py
[hbrs-theta-utils]: https://github.com/JM1/hbrs-theta_utils/
[hbrs-theta-utils-detail-scatter]: https://github.com/JM1/hbrs-theta_utils/blob/master/src/hbrs/theta_utils/detail/scatter/impl.cpp
[hbrs-theta-utils-detail-vtk]: https://github.com/JM1/hbrs-theta_utils/blob/master/src/hbrs/theta_utils/detail/vtk/impl.cpp
[jm]: http://www.jakobmeng.de
[matlab-coder]: https://de.mathworks.com/products/matlab-coder.html
[matlab-help]: https://de.mathworks.com/help/
[podman-install]: https://podman.io/getting-started/installation
[py-edamer]: https://github.com/JM1/python-edamer
[pytest-doc]: https://docs.pytest.org/
[python3-ref]: https://docs.python.org/3/reference/
[tau]: http://tau.dlr.de/
[wiki-cpp17]: https://en.wikipedia.org/wiki/C++17
[wiki-mpi]: https://en.wikipedia.org/wiki/Message_Passing_Interface
[wiki-svd]: https://en.wikipedia.org/wiki/Singular_value_decomposition
