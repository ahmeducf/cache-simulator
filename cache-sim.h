#ifndef CACHE_SIM_H
#define CACHE_SIM_H

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


typedef unsigned char byte;

typedef struct
{   
    unsigned v;     // valid bit
    unsigned tag;
    unsigned time_stamp;
    byte *block;

} Cache_line, *Cache_line_ptr;

typedef struct 
{
    Cache_line_ptr line;
    unsigned counter;

} Cache_set, *Cache_set_ptr;

typedef struct 
{
    Cache_set_ptr set;

} Cache, *Cache_ptr;


/// Construct cache of specified size
/// Return pointer to cache
Cache_ptr init_cache(unsigned S, unsigned E, unsigned B);

/// Parse set-index and tag from given address, based on number of b and s bits
void parse_addressInfo(long long unsigned *indx, long long unsigned *tag, long long address, 
                                unsigned b, unsigned s);
/// Parse arguments from the command line
void parse_arguments(unsigned argc, char* argv[], unsigned *s, unsigned *E, 
                        unsigned *b, char **file_path);

/// Given the index of the set, search that set for an empty line
/// return the index of the first empty line in set[indx]
/// or return -1 if can't find
int find_empty_line(Cache_ptr sim_c, long long unsigned indx, unsigned E);

/// parse block from cache and do the specified operations on it
void hit (Cache_ptr sim_c, long long unsigned indx, unsigned line_indx, unsigned *hits, char *operation);

/// parse block from memory and place it in the cache.
/// place block in empty line if any,
/// if there is no empty line, evect one and place the block instead of it
void miss (Cache_ptr sim_c, long long unsigned indx, int line_indx, long long unsigned tag,
             unsigned E, unsigned *hits, unsigned *misses, unsigned *evictions, char *operation);

void update_time_stamp(Cache_ptr sim_c, long long unsigned indx, unsigned line_indx);

/// Place block in at line[line_indx] of set[indx]
/// change line's tag to block's tag, update time-stamp and set valid-bit
void place_block(Cache_ptr sim_c, long long unsigned indx, unsigned block_tag, unsigned line_indx);

/// return line index of the the Least-Recent-Used line in set[indx]
unsigned queue_top(Cache_ptr sim_c, long long unsigned indx, unsigned E);

/// find Least-Recent-Used line in set[indx] and place the given block at that line
void replace_block(Cache_ptr sim_c, long long unsigned indx, unsigned block_tag, unsigned E);

/// search for line that have tag equal to the given block's tag in set[indx]
/// return line_indx if found such line
/// return -1 otherwise (no line have such tag)
int inspect_cache(Cache_ptr sim_c, long long unsigned indx, long long unsigned tag, unsigned E);

/// open file
/// Return pointer to file if success
/// If the pointer equals NULL, print error message and exit the program with status(1)
FILE * openFile(char *path, char* mode);

/// free all allocated memory spaces
void delete (Cache_ptr sim_c, unsigned S, unsigned E);

#endif