# C++ library for networking


[![build status](https://gitlab.com/VladPodilnyk/netlib/badges/net/pipeline.svg)](https://gitlab.com/VladPodilnyk/netlib/commits/net)
[![coverage report](https://gitlab.com/VladPodilnyk/netlib/badges/net/coverage.svg)](https://gitlab.com/VladPodilnyk/netlib/commits/net)


The library provides classes for network programming.

## Getting Started

### Download the project

```
$ git clone https://github.com/VladPodilnyk/netlib
```

### Build project

Execute instuctions below step by step.
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Build examples

In build directory perform following commands:
```
$ make server
$ make client
$ cp ../examples/gen-rqst.sh ./examples
```
Copying the script is not neccessary but it was made for 
your convinience.

### Run tests

Execute next commands in order to build and run tests.
```
$ make net_test && ./tests/net_test
```


