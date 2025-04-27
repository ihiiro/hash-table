

/* 

    The table size is fixed and assumes no resizing is necessary.
    Open addressing is used, which means collision is resolved via
    a type of probing, linear probing is used with a fixed length of 1.

    Hash function used: FNV-1a 64bit variant.

 */

#include <stdint.h>
#include <sys/types.h>
#include <cstring>

#ifndef FNV_PRIME
 #define FNV_PRIME 1099511628211UL
#endif

#ifndef FNV_OFFSET_BASIS
 #define FNV_OFFSET_BASIS 14695981039346656037UL
#endif

#ifndef PROBE_LENGTH
 #define PROBE_LENGTH 1
#endif

/* 

  the fixed capacity.
  note that with a non fixed capacity you should
  resize the table whenever the load factor approaches
  around half the capacity to prevent operations from
  degenerating into linear performance.

*/
#ifndef M
 #define M 200
#endif

typedef struct {
    const u_char *key;
    const u_char *value;
} bucket;

typedef struct {
    bucket *buckets;
    // uint64_t n; /* # of filled buckets */
} ht;


uint64_t FNV_1A_HASH ( const u_char *k )
{

    uint64_t hash = FNV_OFFSET_BASIS;
    for ( ; *k ; k++ )
    {

        hash ^= *k;
        hash *= FNV_PRIME;

    }

    return hash;

}

uint64_t hash ( const u_char *k )
{

    return FNV_1A_HASH ( k ) % M ;

}

void insert ( ht *HT , const u_char *key , const u_char *value )
{

    uint64_t lvalue = 0;
    uint64_t i = 0;
    u_char *value_copy;
    uint64_t index = hash ( key );

    for (; value [ lvalue ] ; lvalue++ );
    value_copy = new u_char [ lvalue + 1 ];
    for (; i < lvalue ; i++ )
        value_copy [ i ] = value [ i ];
    value_copy [ i ] = '\0';
    /* linear probing with a fixed length */
    while ( 1 )
    {

        if ( index == M )
        {

            index = 0;
            continue;

        }
        if ( HT->buckets [ index ].key == NULL )
        {

            HT->buckets [ index ].key = key;
            HT->buckets [ index ].value = value_copy;
            return;

        }

        index += PROBE_LENGTH;

    }

}

bucket get ( ht *HT , const u_char *key )
{

    uint64_t index = hash ( key );

    while ( 1 )
    {

        if ( index == M )
        {

            index = 0;
            continue;

        }
        if ( std::strcmp ( (char *)key , (char *)HT->buckets [ index ].key  ) == 0 )
            return HT->buckets [ index ];

        index += PROBE_LENGTH;

    }

}


#include <iostream>

int main ( int argc, char **argv )
{
    ht *hash_table = new ht;
    hash_table->buckets = new bucket [ M ];
    // hash_table->n = 0;
    std::memset ( hash_table->buckets , 0 , M );

    u_char *key = (u_char *)"Content-nigga";
    u_char *value = (u_char *)"nigga stuff";
    insert ( hash_table , key , value);

    std::cout << get ( hash_table , key ).key << "\n";
    std::cout << get ( hash_table , key ).value << "\n";



    return 0;

}