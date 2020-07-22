# genetic-algorithm (C++17)
This is a genetic algorithm library. It supports an abstract and open-ended interface.

# Getting Start

First, make the static library `libgenetic.a` or `genetic.lib`.
```
$ git clone https://github.com/asuka1975/genetic-algorithm.git
$ cd genetic-algorithm
$ cmake .
$ make
```
Then, put completed binary file on library directory of your project and put header files on include directory.

# Overview
## genome

You can make a class which behavior as genome. 
However, the class which can behavior as genome should be copy-assignable and invokable `crossover`

The function `crossover` should have a signature as follows.

```c++
TGenome crossover(const TGenome&, const TGenome&);
```
And, `crossover` must be defined in namespace `genetic`.