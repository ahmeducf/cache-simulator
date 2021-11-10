# Cache Simulator
Cache Simulator is a program that Simulates the behavior of a cache memory.

[Computer Systems](https://www.cs.cmu.edu/afs/cs/academic/class/15213-f15/www/index.html) Course Lab.


## Real Cache Structure
- A cache for a machine is organized as an array of **S = 2^s cache sets**. (s: number of set-index bits)

- Each set consists of **E cache lines**.

- Each line consists of a **data block of *B = 2^b* bytes** (b: number of block bits), a **valid bit** that indicates whether or not the line contains meaningful information, and t = m − (b + s) **tag bits** that uniquely identify the block stored in the cache line.

- **Time Stamps** are used to Evict the **least recently used** block from the cache to make room for the next block.


## How does it work?
It takes a valgrind memory trace as input, simulates the hit/miss behavior of a cache memory on this trace, and outputs the total number of ***hits***, ***misses***, and ***evictions***.

It has the following detailed behaviour:

- ***cache-sim*** file contains ***parse_arguments()*** function that parse arguments **(s, E, b)** and **traces file** from command-line.

- The program then takes each valgrind memory trace, parse the ***set-index*** and ***tag*** from the given address through ***parse_addressInfo()*** function.

- After parsing the address, the program calls ***inspect_cache()*** function, which checks whether the set whose index is ***set-index*** has a line that has tag equal to the given ***tag***. If so, it returns the ***line-index***. Otherwise it returns -1.

- if ***inspect_cache()*** returns ***line_index***, then the program calls ***hit()*** function, and parse that block from cache and do the specified operations on it. Otherwise, the program calls ***find_empty_line()*** function that checks whether there is an empty line in the set. if so, the program calls ***miss()*** function that parse the block from memory and place it, in that empty line, in the cache. 

- if there is no empty lines in the set, then evict the **Least Recent Used** one, and place the block instead of it.




## Requirements
- Git
- GCC

## How to test it?

Run the following commands in a terminal:
  - `git clone https://github.com/ahmed-salah-ucf/cache-simulator.git`
  - `make`

You can run cache simulator using different cache parameters and traces.

  - `./csim -s <s> -E <E> -b <b> -t <traces file>`

for example :

  - `./csim -s 1 -E 1 -b 1 -t traces/yi2.trace`

## Thoughts
- The Project really helped me understand cache memories mechanism, how they work, and the impact that cache memories can have on the performance of applications.
- I learned how to write programs that exploit cache memory.