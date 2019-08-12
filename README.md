# cmap
Header only B-tree map in C.

Based on 32-layer bits b-tree and DJBHash hashing algorithm.

# Usage

See `test.c`.

In general ...
```C
#include "cmap.h"
int main(){
  CMap *map = cmap_init();
  int i = 5;
  cmap_add(map, "key", &(i));
  int j = *(int*)cmap_get(map, "key");
  cmap_free(map);
  return 0;
 };
```
