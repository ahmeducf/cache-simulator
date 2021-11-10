#include "cache-sim.h"

Cache_ptr init_cache(unsigned S, unsigned E, unsigned B)
{
    Cache_ptr C_sim = malloc(sizeof(Cache));

    if (C_sim == NULL)
        return NULL;

    Cache_set_ptr set = calloc(S, sizeof(Cache_set));
    if (set == NULL){
        free(C_sim);
        return NULL;
    }
    C_sim->set = set;

    for (size_t i = 0; i < S; i++)
    {
        C_sim->set[i].line = calloc(E, sizeof(Cache_line));

        C_sim->set[i].counter = 0;

        for (size_t j = 0; j < E; j++)
        {
            C_sim->set[i].line[j].v = 0;
            C_sim->set[i].line[j].tag = 0;
            C_sim->set[i].line[j].time_stamp = 0;
            C_sim->set[i].line[j].block = malloc(B * sizeof(byte));
        }
    }
    return C_sim;
}

void parse_addressInfo(long long unsigned *indx, long long unsigned *tag, long long address, 
                                unsigned b, unsigned s)
{
    long long unsigned set_mask = ~((-1) << s);
    *indx = set_mask & (address >> b); 
    *tag = address >> (b + s);
}

void parse_arguments(unsigned argc, char* argv[], unsigned *s, unsigned *E, 
                        unsigned *b, char **file_path)
{
    char c;
    while ((c = (getopt(argc, argv, "s:E:b:t:"))) != -1)
    {
        switch (c)
        {
            case 's':
                *s = atoi(optarg);
                break;
            case 'E':
                *E = atoi(optarg);
                break;
            case 'b':
                *b = atoi(optarg);
                break;
            case 't':
                *file_path = optarg;
                break;
            default:
                printf("wrong argument\n");
                exit(-1);
                break;
        }
    }
}


int find_empty_line(Cache_ptr sim_c, long long unsigned indx, unsigned E)
{
    for (size_t i = 0; i < E; i++)
    {
        unsigned validBit = sim_c->set[indx].line[i].v;
        if (!validBit){
            return i;
        }
    }
    return -1;  
}

void update_time_stamp(Cache_ptr sim_c, long long unsigned indx, unsigned line_indx)
{
    sim_c->set[indx].line[line_indx].time_stamp = ++(sim_c->set[indx].counter);
}

void hit (Cache_ptr sim_c, long long unsigned indx, unsigned line_indx,
            unsigned *hits, char *operation)
{

    char op = *operation;
    switch (op)
    {
        case 'M':
            (*hits) += 2;
            update_time_stamp(sim_c, indx, line_indx);
            break;
        case 'L':
        case 'S':
            (*hits) += 1;
            update_time_stamp(sim_c, indx, line_indx);
            break;
        default:
            break;
    }
}

void miss (Cache_ptr sim_c, long long unsigned indx, int line_indx, long long unsigned tag,
             unsigned E, unsigned *hits, unsigned *misses, unsigned *evictions, char *operation)
{
    if (*operation == 'I') return;

    *misses += 1;
    if (line_indx == -1)
    {
        *evictions += 1;
        replace_block(sim_c, indx, tag, E);
    }
    else
    {
        place_block(sim_c, indx, tag, line_indx);
    }

    if (*operation == 'M') *hits += 1;
}

void place_block(Cache_ptr sim_c, long long unsigned indx, unsigned block_tag, unsigned line_indx)
{
    sim_c->set[indx].line[line_indx].tag = block_tag;
    
    update_time_stamp(sim_c, indx, line_indx);

    sim_c->set[indx].line[line_indx].v = 1;
}


unsigned queue_top(Cache_ptr sim_c, long long unsigned indx, unsigned E)
{
    unsigned min = 0xffffffff;
    unsigned LRU_line_indx;
    for (size_t i = 0; i < E; i++)
    {
        unsigned cnt = sim_c->set[indx].line[i].time_stamp;
        if (cnt < min){
            min = cnt;
            LRU_line_indx = i;
        }
    }
    return LRU_line_indx;
}

void replace_block(Cache_ptr sim_c, long long unsigned indx, unsigned block_tag, unsigned E)
{
    unsigned line_indx = queue_top(sim_c, indx, E);

    place_block(sim_c, indx, block_tag, line_indx);
}


int inspect_cache(Cache_ptr sim_c, long long unsigned indx, long long unsigned tag, unsigned E)
{
    for (size_t i = 0; i < E; i++)
    {
        unsigned validBit = sim_c->set[indx].line[i].v;
        unsigned line_tag = sim_c->set[indx].line[i].tag;
        if (line_tag == tag && validBit){
            return i;
        }
    }
    return -1;  
}

FILE * openFile(char *path, char* mode)
{
    FILE * file = fopen(path, mode);
    if (!file) {
        printf("Error: Cannot open file %s", path);
        exit(1);
    }
    return file;
}

void delete (Cache_ptr sim_c, unsigned S, unsigned E)
{
    for (size_t i = 0; i < S; i++)
    {
        for (size_t j = 0; j < E; j++)
        {
            free(sim_c->set[i].line[j].block);
        }
        
    }
    for (size_t i = 0; i < S; i++)
    {
        free(sim_c->set[i].line);
    }
    free(sim_c->set);
    free(sim_c);
}