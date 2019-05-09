# hbrs-mpl
[![Build Status](https://travis-ci.com/JM1/hbrs-mpl.svg?branch=master)](https://travis-ci.com/JM1/hbrs-mpl)

# How to build this code using Docker

```sh
# install Docker CE for Debian or derivatives
# please follow guide at https://docs.docker.com/install/linux/docker-ce/debian/

# docker version 18.06.0-ce or later is recommended
docker --version

# fetch docker image
docker pull jm1337/debian-dev-hbrs:testing

# log into docker container
docker run -ti jm1337/debian-dev-hbrs:testing



# the following commands are executed from within the docker container

# fetch, compile and install prerequisites
git clone --depth 1 https://github.com/JM1/hbrs-cmake.git
cd hbrs-cmake
mkdir build && cd build/
cmake -DHBRS_CMAKE_ENABLE_TESTS=OFF ..
make -j$(nproc)
sudo make install
cd ../../

# fetch, compile and install hbrs-mpl
git clone --depth 1 https://github.com/JM1/hbrs-mpl.git
cd hbrs-mpl
mkdir build && cd build/
cmake -DHBRS_MPL_ENABLE_TESTS=ON ..
make -j$(nproc)
ctest --output-on-failure
sudo make install
```

For more examples on how to build and test this code see [`.travis.yml`](https://github.com/JM1/hbrs-mpl/blob/master/.travis.yml).
