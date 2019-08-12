#ifndef _CMAP_H_
#define _CMAP_H_

#include "stdlib.h"
#include "string.h"

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

struct node {
    struct node *(child[2]);
    void *value;
} ;

typedef struct node CMap;

static CMap *cmap_newnode(CMap *parent, int bit)
{
    CMap *newnode = (CMap *)malloc(sizeof(CMap));
    newnode->child[0] = newnode->child[1] = NULL;
    newnode->value = NULL;
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
    for (i = 0; i < 2; ++i)
        if (map->child[i])
            cmap_free(map->child[i]);
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
        const int bit = (hash >> i) & 1;
        if (!no->child[bit]) {
            no->child[bit] = cmap_newnode(no, bit);
        }
        no = no->child[bit];
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
        const int bit = (hash >> i) & 1;
        if (no->child[bit])
            no = no->child[bit];
        else
            return NULL;
    }
    return no->value;
}

#endif