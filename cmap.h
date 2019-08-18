#ifndef _CMAP_H_
#define _CMAP_H_

#include "stdlib.h"
#include "string.h"
#include "limits.h"

#define MAX_UINTBITS (sizeof(unsigned int) * CHAR_BIT)

static unsigned int DJBHash(const char* str, unsigned int length)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i) {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}

static unsigned int cmap_get_hash(char* key)
{
    return DJBHash(key, strlen(key));
}

union node {
    union node *(child[2]);
    void *value;
};


typedef union node CMap;

static CMap *cmap_newnode(CMap *parent, int bit)
{
    CMap *newnode = (CMap *)malloc(sizeof(CMap));
    newnode->child[0] = newnode->child[1] = NULL;
    if (parent) {
        parent->child[bit] = newnode;
    };
    return newnode;
}

CMap *cmap_init()
{
    return cmap_newnode(NULL, 0);
};

void cmap_free(CMap *map)
{
    int i;
    if (!map->value)
        for (i = 0; i < 2; ++i)
            if (map->child[i])
                cmap_free(map->child[i]);
    if (map) {
        free(map);
    }
}


CMap *cmap_add_uint(CMap *root, const unsigned int hash, void *value)
{
    if (!root) {
        root = cmap_init();
    }
    CMap *no = root;
    int i = MAX_UINTBITS - 1;
    for (; i >= 0; --i) {
        // iterating over bits
        const int bit = (hash >> i) & 1;
        if (!no->child[bit]) {
            no->child[bit] = cmap_newnode(no, bit);
        }
        no = no->child[bit];
    }
    no->value = value;
    return root;
}

CMap* cmap_add_char(CMap *root, char* key, void *value){
    return cmap_add_uint(root, cmap_get_hash(key), value);
}


void *cmap_get_uint(CMap *root, const unsigned int hash)
{
    if (!root) {
        return NULL;
    }
    // const unsigned int hash = cmap_get_hash(key);
    CMap *no = root;
    int i = MAX_UINTBITS - 1;
    for (; i >= 0; --i) {
        const int bit = (hash >> i) & 1;
        if (no->child[bit])
            no = no->child[bit];
        else
            return NULL;
    }
    return no->value;
}

void *cmap_get_char(CMap* map, char*key){
    return cmap_get_uint(map, cmap_get_hash(key));
}   

#ifndef CMAP_UINT
    #define cmap_add cmap_add_char
    #define cmap_get cmap_get_char
#else
    #define cmap_add cmap_add_uint
    #define cmap_get cmap_get_uint
#endif


#endif