/* 
 * -A cache for a machine is organized as an array of S = 2^s cache sets.
 *
 * -Each set consists of E cache lines.
 * 
 * -Each line consists of a data block of B = 2^b bytes,
 *  a valid bit that indicates whether or not the line contains meaningful information,
 *  and t = m − (b + s) tag bits that uniquely identify the block stored in the cache line.
 * 
 * -Time stamps used to Evict the least recently used block from the cache to make room
 *  for the next block.
 * */

#include "cache-sim.h"

int main(int argc, char* argv[])
{   
    /// s: number of set-index bits
    /// b: number of block bits
    /// S: number of sets in cache
    /// E: associativity per cache-set
    /// B: number of bytes per line
    unsigned s, S, E, b, B;
    char *file_path;
    FILE *pTraceFile; // pointer to trace FILE object


    parse_arguments(argc, argv, &s, &E, &b, &file_path);

    
    S = 1 << s;
    B = 1 << b;
    
    
    Cache_ptr simulated_Cache = init_cache(S, E, B);
    
    
    long long unsigned indx, tag; int line_indx;
    unsigned hits = 0, misses = 0, evictions = 0;
    long long  address; int size;
    char operation;
    pTraceFile = openFile(file_path, "r");

    while(fscanf(pTraceFile, " %c %llx,%d\n", &operation, &address, &size) != EOF)
    {

        if (operation == 'I') continue;
        
        parse_addressInfo(&indx, &tag, address, b, s);

        
        line_indx = inspect_cache(simulated_Cache, indx, tag, E);
        if (line_indx != -1)
        {
            hit(simulated_Cache, indx, line_indx, &hits, &operation);
        }
        else
        {
            line_indx = find_empty_line(simulated_Cache, indx, E);
            miss(simulated_Cache, indx, line_indx, tag, E, &hits, &misses, &evictions, &operation);
        }
    }

    printSummary(hits, misses, evictions);
    
    fclose(pTraceFile);
    delete(simulated_Cache, S, E);
    return 0;
}