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
However, the class which can behavior as genome should be copy-assignable, copy-construcitible, default-constructible and `crossover` invokable 

The function `crossover` should have a signature as follows.

```c++
TGenome crossover(const TGenome&, const TGenome&);
```
And, `crossover` must be defined in namespace `genetic` before `#include<genetic.h>`.

for example,

```c++
#include <string>
namespace genetic {
    std::string crossover(const std::string& d1, const std::string& d2) {
        std::string d = d1;
        for(int i = 0; i < d.size(); i++) {
            if(i < d.size() / 2) d[i] = d2[i];
        }
        return d;
    }
}
#include <genetic.h>
``` 

or

defined in crossover.h
```c++
#include <string>
namespace genetic {
    std::string crossover(const std::string& d1, const std::string& d2) {
        std::string d = d1;
        for(int i = 0; i < d.size(); i++) {
            if(i < d.size() / 2) d[i] = d2[i];
        }
        return d;
    }
}
```

in main.cpp

```c++
#include "crossover.h"
#include <genetic.h>
```

# ga_config
Ga_config means a configuration of genetic algorithm. It has 11 member variables and a member type.

## member types
|name|explanation|
|:---:|:---|
|individual_t|mean a tuple which has genomes|

## member variables
|name|type|explanation|
|:---:|:---:|:---|
|population|std::uint64_t|the number of individuals in an initial state|
|epoch|std::uint64_t|the number of generations|
|fitness_max|float|maximum possible value of fitness|
|fitness_min|float|minimum possible value of fitness|
|callback|std::function<void>(const std::vector\<float\>&)|always called every time learning of one generation has finished|
|select|std::function<std::vector<individual_t>(const std::vector<individual_t>&, const std::vector<float>&)>|generate and select new individuals from old individuals by fitness values|
|step|std::function<std::vector<float>(const std::vector<individual_t>&)>|individuals do task|
|scale|std::function<float(float)>|change scale of fitness value|
|initializer|std::function<individual_t()>|initialize per one individual|
|mutates|std::vector<std::pair<float, std::function<void(individual_t&)>>>|have a pair of a probability and mutate function of genome|
|node_mutates|std::vector<std::pair<float, std::function<void(float, individual_t&)>>>|if a genome has a structure which mean a kind of container, have a pair of a probability and mutate function of per node of a genome|
