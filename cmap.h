#ifndef _CMAP_H_
#define _CMAP_H_

#include "stdlib.h"
#include "string.h"

//murmur stuff

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

static unsigned int MurmurHash2A ( const void *key, int len, unsigned int seed )
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    unsigned int l = len;

    const unsigned char *data = (const unsigned char *)key;

    unsigned int h = seed;
    unsigned int k;

    while(len >= 4) {
        k = *(unsigned int *)data;

        mmix(h, k);

        data += 4;
        len -= 4;
    }

    unsigned int t = 0;

    switch(len) {
    case 3:
        t ^= data[2] << 16;
    case 2:
        t ^= data[1] << 8;
    case 1:
        t ^= data[0];
    };

    mmix(h, t);
    mmix(h, l);

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
};

static unsigned int cmap_get_hash(char* key)
{
    return MurmurHash2A(key, strlen(key), 0);
}

struct node {
    struct node *one, *zero;
    void *value;
} ;

typedef struct node CMap;

static CMap *cmap_newnode(CMap *parent, char bit)
{
    CMap *newnode = (CMap *)malloc(sizeof(CMap));
    newnode->one = newnode->zero = NULL;
    newnode->value = NULL;
    if (parent) {
        if (bit) {
            parent->one = newnode;
        } else {
            parent->zero = newnode;
        }
    };
    return newnode;
}

CMap *cmap_init()
{
    return cmap_newnode(NULL, 0);
};

void cmap_free(CMap *map)
{
    if (map->one) {
        cmap_free(map->one);
    }
    if (map->zero) {
        cmap_free(map->zero);
    }
    if (map) {
        free(map);
    }
}

CMap *cmap_add(CMap *root, char *key, void *value)
{
    const unsigned int hash = cmap_get_hash(key);
    if (!root) {
        root = cmap_init();
    }
    CMap *no = root;
    int i = sizeof(unsigned int) * 8 - 1;
    for (; i >= 0; --i) {
        // iterating over bits
        const char bit = (hash >> i) & 1;
        if (bit) {
            if (!no->one) {
                no->one = cmap_newnode(no, bit);
            };
            no = no->one;
        } else {
            if (!no->zero) {
                no->zero = cmap_newnode(no, bit);
            }
            no = no->zero;
        }
    }
    no->value = value;
    return root;
}

void *cmap_get(CMap *root, char *key)
{
    if (!root) {
        return NULL;
    }
    const unsigned int hash = cmap_get_hash(key);
    CMap *no = root;
    int i = sizeof(unsigned int) * 8 - 1;
    for (; i >= 0; --i) {
        const char bit = (hash >> i) & 1;
        if (bit) {
            if (no->one) {
                no = no->one;
            } else {
                return NULL;
            }
        } else {
            if (no->zero) {
                no = no->zero;
            } else {
                return NULL;
            }
        }
    }
    return no->value;
}

#endif